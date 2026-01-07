#include "test_game_1_contents_main.hpp"

Test_Game_1_Main_Loop::Test_Game_1_Main_Loop() : 
main_shader_manager("shader/main_shader.vert", "shader/main_shader.frag", 20),
emulated_bone(64)
{

}

Speed_Charm_3D_Main * sc3 = nullptr;
Speed_Charm_3D_Run_Main * sc3_rmain = nullptr;
struct Speed_Charm_3D_Virtual_Vertex_Package : public Basic_Gl_Vertex_Package<3, GL_DYNAMIC_DRAW>{

};
static Speed_Charm_3D_Virtual_Vertex_Package * delete_glsys_vp(Speed_Charm_3D_Virtual_Vertex_Package * sc3_rmain){
    delete []sc3_rmain;
    return nullptr;
}

int Test_Game_1_Main_Loop::init_loop(){

    sample_light.color_manager(1.0f);
    sample_light.pos_manager(0.0f, 50.0f, 60.0f);
    glfw_sub_manager.glfw_manager->set_cursor_basic_cursor(cursor_manager);

    main_shader_manager.register_shader_var_name_id(E_Main_Shader_Projection, "projection");
    main_shader_manager.register_shader_var_name_id(E_Main_Shader_View_Pos, "view");
    main_shader_manager.register_shader_var_name_id(E_Main_Shader_Skin_Bones, "skin_bones");
    main_shader_manager.register_shader_var_name_id(E_Main_Shader_Model, "model");
    
    main_shader_manager.register_shader_var_name_id(E_Main_Shader_Light_Pos, "lightPos");
    main_shader_manager.register_shader_var_name_id(E_Main_Shader_Light_Color, "lightColor");
    main_shader_manager.register_shader_var_name_id(E_Main_Shader_Mesh_Color, "objectColor");
    
    main_shader_manager.register_shader_var_name_id(E_Main_Shader_Static_Check, "static_check");
    main_shader_manager.register_shader_var_name_id(E_Main_Shader_Frag_Check, "shader_num");
    main_shader_manager.register_shader_var_name_id(E_Main_Shader_Light_View, "light_view");
    main_shader_manager.register_shader_var_name_id(E_Main_Shader_Shadow_Map, "shadowMap");
    sc_err_t err_code;
    err_code = read_speed_charm_3d_main(&sc3, "a");
    if(err_code < SC3_FILE_OUT_CODE){
        throw std::runtime_error("Error : Failed to load sc3 file" + std::to_string(err_code));
    }
    Speed_Charm_3D_Virtual_Vertex_Package * sc3_vvp = new Speed_Charm_3D_Virtual_Vertex_Package[sc3->meshes_num];

    bsc3.resize(sc3->meshes_num);
    for(size_t i = 0; i < sc3->meshes_num; ++i){
        sc3_vvp[i].gen_vp();
        sc3_vvp[i].set_up_vp_buffer_on_gpu(0, sc3->sc_meshes[i].mesh_attr, sc3->sc_meshes[i].attr_num);
        sc3_vvp[i].set_up_vp_buffer_shader_attribute_memory_s(&Speed_Charm_3D_Mesh_Attr::pos, 0, 3);
        sc3_vvp[i].set_up_vp_buffer_shader_attribute_memory_s(&Speed_Charm_3D_Mesh_Attr::norm, 1, 3);
        if(sc3->sc_meshes[i].skin_attr){
            sc3_vvp[i].set_up_vp_buffer_on_gpu(1, sc3->sc_meshes[i].skin_attr, sc3->sc_meshes[i].attr_num);
            sc3_vvp[i].set_up_vp_buffer_shader_attribute_memory_is<sc_ushort>(&Speed_Charm_3D_Skin_Attr::skin_indices, 4, 4);
            sc3_vvp[i].set_up_vp_buffer_shader_attribute_memory_s
            <sc_ubyte, GL_TRUE>(&Speed_Charm_3D_Skin_Attr::skin_weight, 5, 4);

        }
        sc3_vvp[i].set_up_vp_ebo_on_gpu(2, sc3->sc_meshes[i].indices, sc3->sc_meshes[i].ind_num);
        
    }
    sc3_rmain = change_sc3_main_to_sc3_run_main(&sc3, sc3_vvp);
    if(!sc3_rmain){
        delete_glsys_vp(sc3_vvp);
        delete_speed_charm_3d_main(&sc3);
        throw std::runtime_error("Error : Failed to load sc3 file" + std::to_string(err_code));
    }
    std::cout << "Start" << std::endl;
    return 1;
}

int Test_Game_1_Main_Loop::main_loop(){
    cursor_manager.camera_control.move_camera(* glfw_sub_manager.glfw_manager);
    cursor_manager.cursor_is_valid_program(* glfw_sub_manager.glfw_manager);
    SGG_Vector3 center = cursor_manager.camera_control.get_camera_center();
    const SGG_Vector3 & pos = cursor_manager.camera_control.c_pos;
    const SGG_Vector3 & up = cursor_manager.camera_control.c_up;
    SGG_Mat4 lookAt;
    SGG_Mat4 model;
    SGG_Mat4 proj;
    sgg_lookAt(lookAt, pos, center, up);
    sgg_identity(&model);
    sgg_perspective(&proj, 45.0f, 1600.0f / 900.0f, 0.05f, 200.0f);
    SGG_Mat4 light_proj_view;
    glfw_sub_manager.render_base_color(0.0f, 1.0f, 1.0f);
    glfw_sub_manager.begin_3D_mask_mode();
    main_shader_manager.begin_program_shader();
    Basic_Camera3D_Manager<E_Main_Shader_View_Pos, E_Main_Shader_Projection>::up_load_perspective(main_shader_manager, 45.0f, 1600.0f / 900.0f, 0.05f, 200.0f);
    obj_color.use_obj_color(main_shader_manager);
    sample_light.use_light(main_shader_manager);
    main_shader_manager.begin_program_shader();
    Basic_Camera3D_Manager<E_Main_Shader_View_Pos, E_Main_Shader_Projection>::up_load_look_at(main_shader_manager, lookAt);
    Basic_Camera3D_Manager<E_Main_Shader_View_Pos, E_Main_Shader_Projection>::up_load_perspective(main_shader_manager, proj);
    main_shader_manager.upload_shader_uniform_data(E_Main_Shader_Frag_Check, 0);
    use_static_check<E_Main_Shader_Frag_Check>(main_shader_manager, 0);
    emulated_bone.start_emulated_u2d_data(main_shader_manager);
    start_animation_speed_charm_3d_bone(sc3_rmain, 0, 0.01f);
    start_speed_charm_3d_bones_hierarchy(sc3_rmain);
    std::vector<SGG_Mat4> gms;
    for (size_t i = 0; i < sc3_rmain->bones_num; ++i) {
        SGG_Mat4 new_mat;
        start_build_bones_speed_charm_3d(sc3_rmain, &sc3_rmain->sc_bones[i], &new_mat);
        gms.push_back(new_mat);
    }
    emulated_bone.use_emulated_u2d_data((void *)gms.data(), gms.size());

    for (size_t i = 0; i < sc3_rmain->meshes_num; ++i) {
        if(have_skin_check(&sc3_rmain->sc_meshes[i])){
            use_static_check<E_Main_Shader_Static_Check>(main_shader_manager, 0);
        }
        else{
            use_static_check<E_Main_Shader_Static_Check>(main_shader_manager, 1);
        }
        sc3_rmain->sc_vvp[i].e_draw();
    }
    return glfw_sub_manager.force_end();
}

int Test_Game_1_Main_Loop::end_loop(){
    delete_speed_charm_3d_main(&sc3);
    delete_speed_charm_3d_run_main(&sc3_rmain, delete_glsys_vp);
    std::cout << "END" << std::endl;
    return 1;
}
