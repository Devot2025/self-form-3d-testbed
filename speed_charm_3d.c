#include "speed_charm_3d.h"

static sc_err_t delete_speed_charm_3d_meshes(Speed_Charm_3D_Mesh * sc3_meshes, sc_size mesh_size){
    if(!sc3_meshes) return SC3_FILE_OUT_CODE;
    for(sc_size i = 0; i < mesh_size; ++i){
        s_free((*(sc3_meshes + i)).indices);
        s_free((*(sc3_meshes + i)).mesh_attr);
        s_free((*(sc3_meshes + i)).skin_attr);
        //s_free(__sc3_main->sc_meshes[i].mesh_bone_map);
    }
}
static sc_err_t delete_speed_charm_3d_anim_deform(Speed_Charm_3D_Anim_Deform  * sc3_anim_def, sc_size skeletons_size){
    if(!sc3_anim_def) return SC3_FILE_OUT_CODE;
    for(sc_size i = 0; i < skeletons_size; ++i){
        s_free((*(sc3_anim_def + i)).pos);
        s_free((*(sc3_anim_def + i)).rot);
        s_free((*(sc3_anim_def + i)).scale);
    }
}
static sc_err_t delete_speed_charm_3d_anim(Speed_Charm_3D_Anim * sc3_anim, sc_size anim_size, sc_size skeletons_size){
    if(!sc3_anim) return SC3_FILE_OUT_CODE;
    for(sc_size i = 0; i < anim_size; ++i){
        delete_speed_charm_3d_anim_deform(sc3_anim->anim_deforms, skeletons_size);
        s_free((*(sc3_anim + i)).anim_deforms);
    }

}
sc_err_t delete_speed_charm_3d_main(Speed_Charm_3D_Main ** sc3_main){
    if(!sc3_main) return SC3_FILE_OUT_CODE;
    Speed_Charm_3D_Main* sc3_main__ = *sc3_main;
    if(!sc3_main__) return SC3_FILE_OUT_CODE;
    delete_speed_charm_3d_meshes(sc3_main__->sc_meshes, sc3_main__->meshes_num);
    delete_speed_charm_3d_anim(sc3_main__->sc_anim, sc3_main__->anim_num, sc3_main__->skeletons_num);
    s_free(sc3_main__->sc_bones);
    s_free(sc3_main__->sc_meshes);
    s_free(sc3_main__->sc_anim);
    s_free(sc3_main__);
    return SC3_FILE_OUT_CODE;
}
sc_err_t delete_speed_charm_3d_main_contents(Speed_Charm_3D_Main_Contents ** sc3_main_cont){
    if(!sc3_main_cont) return SC3_FILE_OUT_CODE;
    if(!*sc3_main_cont) return SC3_FILE_OUT_CODE;
    
    delete_speed_charm_3d_main(&(*sc3_main_cont)->sc3_main);
    s_free(*sc3_main_cont);
    return SC3_FILE_OUT_CODE;
}

sc_err_t delete_speed_charm_3d_run_main(Speed_Charm_3D_Run_Main ** sc3_run_main, free_vvp_api_funcs s_free_vvp){
    /*specialty error.*/
    if(!s_free_vvp) return SC3_FREE_FUNC_API_NONE;

    if(!sc3_run_main) return SC3_FILE_OUT_CODE;
    Speed_Charm_3D_Run_Main* sc3_run_main__ = *sc3_run_main;
    if(!sc3_run_main__) return SC3_FILE_OUT_CODE;
    delete_speed_charm_3d_anim(sc3_run_main__->sc_anim, sc3_run_main__->anim_num, sc3_run_main__->skeletons_num);
    s_free(sc3_run_main__->sc_bones);
    s_free(sc3_run_main__->sc_meshes);
    s_free(sc3_run_main__->sc_anim);
    s_free_vvp(sc3_run_main__->sc_vvp);
    s_free(sc3_run_main__);
    return SC3_FILE_OUT_CODE;
}

void set_sc3_file_format(sc_byte * sc3_path){
    sc_size name_len__ = safety_strlen(sc3_path);
    sc_size format_offset__ = name_len__;
   const sc_byte * sc3_format__ = ".sc3";
   //const sc_size sc3_size__ = STRING_LITERAL_SIZE(".sc3");
   if(SC3_FILE_NAME_DEFAULT_SIZE <= name_len__) name_len__ = SC3_FILE_NAME_DEFAULT_SIZE - SC_POLICY_FILE_FORMAT_MAX;
   if(SC3_FILE_NAME_DEFAULT_SIZE - name_len__ < SC_POLICY_FILE_FORMAT_MAX) name_len__ = SC3_FILE_NAME_DEFAULT_SIZE - SC_POLICY_FILE_FORMAT_MAX;
   format_offset__ = name_len__;
   
   smart_memcpy_p(sc3_path + format_offset__, sc3_format__, sc_byte, SC_POLICY_FILE_FORMAT_MAX);
   
}
sc_err_t delete_invalid_data(Speed_Charm_3D_Main * sc3_main){
    if(!sc3_main) return SC_POLICY_ERROR_ALLOC;
    //if(!sc3_main) return SC_POLICY_ERROR_ALLOC;
    //if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC_POLICY_ERROR_INVALID_VERSION;
    sc_size anim_idx__ = 0;
    for(sc_size i = 0; i < sc3_main->anim_num; ++i){
        sc_size frame_num__ = sc3_main->sc_anim[i].frame_num;
        float framerate__ = sc3_main->sc_anim[i].framerate;
        if((frame_num__ >= SC3_MAX_FRAME_NUM || framerate__ >= SC3_FRAMERATE_60) && sc3_main->sc_anim[i].anim_deforms){
            for(sc_size j = 0; j < sc3_main->skeletons_num; ++j){
                s_free(sc3_main->sc_anim[i].anim_deforms[j].pos);
                s_free(sc3_main->sc_anim[i].anim_deforms[j].rot);
                s_free(sc3_main->sc_anim[i].anim_deforms[j].scale);
            }
            s_free(sc3_main->sc_anim[i].anim_deforms);
        }
        else{
            sc3_main->sc_anim[anim_idx__++] = sc3_main->sc_anim[i];
        }
    }
    sc3_main->anim_num = anim_idx__;
    return SC3_FILE_OUT_CODE;
}
static sc_err_t solution_sc3_skeleton_sort(Speed_Charm_3D_Main_Contents * sc3_main_cont){
    if(!sc3_main_cont) return SC_POLICY_ERROR_ALLOC;
    if(!sc3_main_cont->sc3_main) return SC_POLICY_ERROR_ALLOC;

    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC_POLICY_ERROR_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;

    /**
     * 0 1 2 3 4 5 6 7
     *   0 1 2 1 7 2 3
     * 5 > 7
    */
    bool check___ = true;
    for (sc_size j = 0; j < sc3_main_cont->sc3_main->skeletons_num; ++j) {
        sc_int p = sc3_main_cont->sc3_main->sc_skeletons[j].parent;
        if (p >= 0 && (sc_size)p > j) {
            check___ = false;
        }
    }
    if(check___) return SC3_FILE_OUT_CODE;
    sc_int * none_root_mapping__ = smart_calloc(sc_int, sc3_main_cont->sc3_main->skeletons_num);
    sc_int * bones_order__ = smart_malloc(sc_int, sc3_main_cont->sc3_main->skeletons_num);
    Speed_Charm_3D_Skeleton * tmp__ = smart_malloc(Speed_Charm_3D_Skeleton, sc3_main_cont->sc3_main->skeletons_num);
    sc_int * new_index__ = smart_malloc(sc_int, sc3_main_cont->sc3_main->skeletons_num);
    if(!none_root_mapping__) return SC_POLICY_ERROR_ALLOC;
    if(!tmp__) return SC_POLICY_ERROR_ALLOC;
    if(!bones_order__) return SC_POLICY_ERROR_ALLOC;
    if(!new_index__) return SC_POLICY_ERROR_ALLOC;
    

    for(sc_size i = 0; i < sc3_main_cont->sc3_main->skeletons_num; ++i){
        sc_int parent = sc3_main_cont->sc3_main->sc_skeletons[i].parent;
        if(parent >= 0 && parent < sc3_main_cont->sc3_main->skeletons_num) none_root_mapping__[i]++;
    }
    
    sc_uint r_idx = 0;
    for(sc_size i = 0; i < sc3_main_cont->sc3_main->skeletons_num; ++i){
        /*collect root bones*/
        if(none_root_mapping__[i] == 0) bones_order__[r_idx++] = (sc_int)i;
    }
    
    /**
     * all skeleton 10
     * 
    */
    for (sc_size i = 0; i < r_idx; ++i) {
        sc_int v = bones_order__[i];
        for (sc_size j = 0; j < sc3_main_cont->sc3_main->skeletons_num; ++j) {
            if (sc3_main_cont->sc3_main->sc_skeletons[j].parent == v) {
                if (--none_root_mapping__[j] == 0) {
                    bones_order__[r_idx++] = (sc_int)j;
                }
            }
        }
    }
    /**
     * 0  1 2 3 4 5 6 7
     * -1 0 1 2 3 7 1 2
     * 
    */
    if(r_idx != sc3_main_cont->sc3_main->skeletons_num){
        s_free(new_index__);
        s_free(tmp__);
        s_free(bones_order__);
        s_free(none_root_mapping__);
        return SC3_FILE_OUT_CODE;
    }
     for (sc_size i = 0; i < sc3_main_cont->sc3_main->skeletons_num; ++i) {
        new_index__[bones_order__[i]] = (sc_int)i;
        tmp__[i] = sc3_main_cont->sc3_main->sc_skeletons[bones_order__[i]];
    }

    for (sc_size i = 0; i < sc3_main_cont->sc3_main->skeletons_num; ++i) {
        sc_int p = tmp__[i].parent;
        if (p >= 0 && (sc_size)p < sc3_main_cont->sc3_main->skeletons_num) {
            tmp__[i].parent = new_index__[p];
        } else {
            tmp__[i].parent = -1;
        }
    }
    smart_memcpy_p(sc3_main_cont->sc3_main->sc_skeletons, tmp__, Speed_Charm_3D_Skeleton, sc3_main_cont->sc3_main->skeletons_num);
    s_free(new_index__);
    s_free(tmp__);
    s_free(bones_order__);
    s_free(none_root_mapping__);

    return SC3_FILE_OUT_CODE;
    
}
static sc_err_t optimize_sc3_mains(Speed_Charm_3D_Main_Contents * sc3_main_cont){
    if(!sc3_main_cont) return SC_POLICY_ERROR_ALLOC;
    if(!sc3_main_cont->sc3_main) return SC_POLICY_ERROR_ALLOC;
    Speed_Charm_3D_Main * sc3_main = sc3_main_cont->sc3_main;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC_POLICY_ERROR_INVALID_VERSION;
    if(!sc3_main) {
        //printf("debubg outn1\n");

        return SC3_FILE_ERROR_VAR_NULL;
    }
    //printf("debubg outn2 %u\n", sc3_main->meshes_num);

    sc_size num__ = 0;

    for(sc_size i = 0; i < sc3_main->meshes_num; ++i){
        //printf("debubg out\n");
        if(sc3_main->sc_meshes[i].attr_num && sc3_main->sc_meshes[i].ind_num && sc3_main->sc_meshes[i].mesh_attr && sc3_main->sc_meshes[i].indices){
            
            sc3_main->sc_meshes[num__] = sc3_main->sc_meshes[i];
            num__++;

        }
        else{
            s_free(sc3_main->sc_meshes[i].mesh_attr);
            s_free(sc3_main->sc_meshes[i].indices);
            //s_free(sc3_main->sc_meshes[i].mesh_bone_map);
        }
    }
    Speed_Charm_3D_Mesh * sc3_meshes__ = smart_realloc(Speed_Charm_3D_Mesh, sc3_main->sc_meshes, num__);
    if(!num__) {
        sc3_main->sc_meshes = sc3_meshes__;
        return SC3_FILE_OUT_CODE;
    }
    

    
    if(!sc3_meshes__) return SC_POLICY_ERROR_ALLOC;
    sc3_main->meshes_num = num__;
    sc3_main->sc_meshes = sc3_meshes__;
    
    for(sc_size i = 0; i < sc3_main->meshes_num; ++i){
        sc_size offset__ = i % 8;
        sc_size idx__ = i / 8;
        sc_ubyte inf = sc3_main->sc_meshes[i].mesh_inf << offset__;
        sc3_main->sc_meshes[idx__].mesh_inf |= inf;
    }
    
    for(sc_size i = 0; i < sc3_main->anim_num; ++i){
        //printf("%u\n", sc3_main->bones_num);
        for(sc_size j = 0; j < sc3_main->skeletons_num; ++j){
            Speed_Charm_3D_Anim_Deform * anim_deform__ = &sc3_main->sc_anim[i].anim_deforms[j];
            //printf("%u ", anim_deform__->deform_status);
            
            if((anim_deform__->deform_status & (~0u << 4)) && j == 0) {
                
                break;
            }
            /**1       0
             * 0000   0000
             * 0 -> 0 04
             * 1 -> 0 40
             * 2 -> 1 04
             * 3 -> 1 40
             * 1111 1111
             * 0000 1111
             * 0
             * 0000 1111
             * 
             * 1010 1010
             * 0000 1111
             * 1111 0000
             * 
            */
            /*
            const sc_size offset__ = (i % 2) * 4;
            const sc_ubyte tmask__ = ((~0u >> 4) << (((1 - i) % 2) << 4));
            sc_ubyte mask__ = (anim_deform__->deform_status & tmask__) << ((i % 2) * 4);
            */
            /**
             * 0
             * 0101 0011
             * 0000 1111
             * 
             * 1
             * 0000 0111
             * 1 % 2 * 4 = 4
             * 1111 1111
             * 0000 1111
             * 1111 0000
             * 0000 0111
             * 0111 0000
            */
            sc_ubyte sts = (~(sc_ubyte)0u >> 4u) << ((j % 2) * 4);
            sc_ubyte sts__ = (~(sc_ubyte)0u >> 4u) << ((1 - j % 2) * 4);
            sc_ubyte top__ =  ((anim_deform__->deform_status) << ((j % 2) * 4)) & sts;
            
            sc_size idx__ = j / 2;
            //printf("%lu ", idx__);
            
            sc3_main->sc_anim[i].anim_deforms[idx__].deform_status &= sts__;
            sc3_main->sc_anim[i].anim_deforms[idx__].deform_status |= top__;
            //printf("[%u : %lu]\n", idx__, sc3_main->sc_anim[i].anim_deforms[idx__].deform_status);
        }
    }

    sc_size opt_deform_num__ = (sc3_main->bones_num + 1) / 2;
    return SC3_FILE_OUT_CODE;
    /*
    for(sc_size i = 0; i < sc3_main->meshes_num; ++i){
        if(sc3_main->sc_meshes[i].attr_num){
            sc3_main->sc_meshes[num__] = sc3_main->sc_meshes[i];
            num__++;
        }
        else{
            s_free(sc3_main->sc_meshes[i].mesh_attr);
            s_free(sc3_main->sc_meshes[i].indices);
            s_free(sc3_main->sc_meshes[i].mesh_bone_map);
        }
    }
    */
}

Speed_Charm_3D_Run_Main * change_sc3_main_to_sc3_run_main(Speed_Charm_3D_Main ** sc3_main, struct Speed_Charm_3D_Virtual_Vertex_Package * vvp){
    if(!sc3_main) return NULL;
    if(!vvp) return NULL;
    Speed_Charm_3D_Main * sc3_main__ = *sc3_main;
    if(!sc3_main__) return NULL;
    Speed_Charm_3D_Run_Main * sc3_run_main__ = smart_calloc(Speed_Charm_3D_Run_Main, 1);
    if(!sc3_run_main__) return NULL;
    sc3_run_main__->sc_meshes = smart_calloc(Speed_Charm_3D_Run_Mesh, sc3_main__->meshes_num);
    if(!sc3_run_main__->sc_meshes){
        /*delete implement*/
        s_free(sc3_run_main__);
        //delete_speed_charm_3d_run_main(sc_run_main__, sc_main__);
        return NULL;
    }
    sc3_run_main__->meshes_num = sc3_main__->meshes_num;
    for(sc_size i = 0; i < sc3_main__->meshes_num; ++i) {    
        sc3_run_main__->sc_meshes[i].mesh_inf = sc3_main__->sc_meshes[i].mesh_inf;
        //sc3_run_main__->sc_meshes[i].mesh_bone_map = sc3_main__->sc_meshes[i].mesh_bone_map;
        //sc3_run_main__->sc_meshes[i].mesh_bone_map_num = sc3_main__->sc_meshes[i].mesh_bone_map_num;
        //sc3_main__->sc_meshes[i].mesh_bone_map = NULL;
    }
    sc3_run_main__->sc_vvp = vvp;
    //printf("anim time : %lf\n", sc_run_main__->anim_times);
    //sc_run_main__->anim_times = 0.0f;
    sc3_run_main__->sc_skeletons = sc3_main__->sc_skeletons;
    sc3_main__->sc_skeletons = NULL;
    sc3_run_main__->skeletons_num = sc3_main__->skeletons_num;
    sc3_run_main__->bones_num = sc3_main__->bones_num;
    sc3_main__->bones_num = 0;
    sc3_run_main__->sc_bones = sc3_main__->sc_bones;
    sc3_main__->sc_bones = NULL;
    sc3_run_main__->sc_anim = sc3_main__->sc_anim;
    sc3_main__->sc_anim = NULL;
    sc3_run_main__->anim_num = sc3_main__->anim_num;
    sc3_main__->anim_num = 0;
    
    //sc_run_main__->sc_vvp = vvp;
    delete_speed_charm_3d_main(sc3_main);

    return sc3_run_main__;
}

Speed_Charm_3D_Main_Contents * init_speed_charm_3d_main_contents(Speed_Charm_3D_Main * sc3_main){
    Speed_Charm_3D_Main_Contents * sc3_main_cont = smart_calloc(Speed_Charm_3D_Main_Contents, 1);
    if(!sc3_main_cont) return NULL;
    if(sc3_main){
        sc3_main_cont->sc3_main = sc3_main;
        return sc3_main_cont;
    }
    sc3_main_cont->sc3_main = smart_calloc(Speed_Charm_3D_Main, 1);
    if(!sc3_main_cont->sc3_main){
        s_free(sc3_main_cont);
        return NULL;
    }
    return sc3_main_cont;
}
sc_err_t write_speed_charm_3d_file_entry(Speed_Charm_3D_Main_Contents * sc3_main_cont, const sc_byte * sc3_file_path, sc_ushort version){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;

    if(!sc3_file_path) sc3_file_path = SC3_DEFAULT_FILE_NAME;

    sc_byte sc3_file_path__[SC3_FILE_NAME_DEFAULT_SIZE] = {0};
    
    if(!ext_str_cpy(sc3_file_path__, sc3_file_path, SC3_FILE_NAME_DEFAULT_SIZE, (safety_strlen(sc3_file_path)) + 1)) return SC3_FILE_ERROR_VAR_NULL;

    set_sc3_file_format(sc3_file_path__);
    
    if(scl_fopen(sc3_main_cont->sc3_f__, sc3_file_path__, "wb")) return SC3_FILE_ERROR_CODE;
    sc3_main_cont->sc3_version = version;

    scl_fwrite(sc3_main_cont->sc3_f__, SC3_FILE_ENTRY_MAGIC_NUMBER, sc_byte, len_of_str(SC3_FILE_ENTRY_MAGIC_NUMBER));

    scl_fwrite(sc3_main_cont->sc3_f__, &version, sc_ushort, 1);
    
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;

    scl_fwrite(sc3_main_cont->sc3_f__, SC3_MAIN_BEG, sc_byte, len_of_str(SC3_MAIN_BEG));
        
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_MAIN_TABLE, sc_byte, len_of_str(SC3_MAIN_TABLE));
        
    return SC3_FILE_OUT_CODE;
}

sc_err_t write_speed_charm_3d_table_helper(Speed_Charm_3D_Main_Contents * sc3_main_cont, const sc_byte * table_str, size_t table_str_len){
    const sc_size booking_table_pos__ = 0;
    scl_fwrite(sc3_main_cont->sc3_f__, table_str, sc_byte, table_str_len);
    scl_fwrite(sc3_main_cont->sc3_f__, &booking_table_pos__, sc_size, 1);
    return SC3_FILE_OUT_CODE;
}
sc_err_t write_speed_charm_3d_main_table(Speed_Charm_3D_Main_Contents * sc3_main_cont){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;

    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC_POLICY_ERROR_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;
    
    write_speed_charm_3d_table_helper(sc3_main_cont, SC3_MAIN_TABLE_MESH, len_of_str(SC3_MAIN_TABLE_MESH));
    write_speed_charm_3d_table_helper(sc3_main_cont, SC3_MAIN_TABLE_BONE, len_of_str(SC3_MAIN_TABLE_BONE));
    write_speed_charm_3d_table_helper(sc3_main_cont, SC3_MAIN_TABLE_ANIM, len_of_str(SC3_MAIN_TABLE_ANIM));
    if(sc3_main_cont->sc3_version >= SC3_TEXTURE_VALID_VERSION) write_speed_charm_3d_table_helper(sc3_main_cont, SC3_MAIN_TABLE_TEXTURE, len_of_str(SC3_MAIN_TABLE_TEXTURE));
    return SC3_FILE_OUT_CODE;
}

sc_err_t write_speed_charm_3d_user(Speed_Charm_3D_Main_Contents * sc3_main_cont, const sc_byte * comment_str){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC_POLICY_ERROR_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;

    //if(!SC_POLICY_CHECK_NONE_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC_POLICY_ERROR_INVALID_VERSION;
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_MAIN_USER, sc_byte, len_of_str(SC3_MAIN_USER));
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_MAIN_COMMENT, sc_byte, len_of_str(SC3_MAIN_COMMENT));
    sc_byte sc3_comment__[SC3_COMMENT_SIZE_MAX] = {0};
    
    set_os_string(sc3_comment__);
    
    if(!comment_str) smart_memcpy_p(sc3_comment__ + SC_POLICY_OS_STR_SIZE, SC3_DEFAULT_COMMENT, sc_byte, SC3_DEFAULT_COMMENT_LEN);
    else{
        sc_size len__ = strlen(comment_str);
        len__ = SC3_DEFAULT_COMMENT_LEN <= len__ ? SC3_DEFAULT_COMMENT_LEN : len__;
        smart_memcpy_p(sc3_comment__ + SC_POLICY_OS_STR_SIZE, comment_str, sc_byte, len__);
    }

    scl_fwrite(sc3_main_cont->sc3_f__, sc3_comment__, sc_byte, SC3_COMMENT_SIZE_MAX);
    
    sc_ubyte fd_data__[SC3_FREE_DATA_SIZE] = {0};
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_FREE_DATA_CHUNK, sc_byte, len_of_str(SC3_FREE_DATA_CHUNK));
    scl_fwrite(sc3_main_cont->sc3_f__, fd_data__, sc_ubyte, SC3_FREE_DATA_SIZE);
    
    return SC3_FILE_OUT_CODE;
}

sc_err_t write_speed_charm_3d_close(Speed_Charm_3D_Main_Contents * sc3_main_cont){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) {
        scl_close(sc3_main_cont->sc3_f__);
        return SC_POLICY_ERROR_INVALID_VERSION;
    }
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) {
        scl_close(sc3_main_cont->sc3_f__);
        return SC3_INVALID_VERSION;
    }
    scl_fgoto_e(sc3_main_cont->sc3_f__);
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_MAIN_END, sc_byte, len_of_str(SC3_MAIN_END));
    scl_close(sc3_main_cont->sc3_f__);
    return SC3_FILE_OUT_CODE;
}
void goto_sc3f_pos(SC_FILE __sc3f, sc_size pos){
    /*now : 10
    * pos : 5
    * now : 5
    * now : 10
    */
    scl_fgoto(__sc3f, pos, SEEK_SET);
}

sc_err_t write_speed_charm_3d_main(const sc_byte * sc3_file_path, Speed_Charm_3D_Main * sc3_main){
    Speed_Charm_3D_Main_Contents sc3_main_cont = {0};
    sc3_main_cont.sc3_main = sc3_main;
    //init_speed_charm_3d_main_contents(sc3_main);
    sc_err_t error_code__ = write_speed_charm_3d_file_entry(&sc3_main_cont, sc3_file_path, SC3_NOW_VERSION);
    if(error_code__ < SC3_FILE_OUT_CODE) {
        //fclose(sc3_main_cont->sc3_f__);
        //delete_speed_charm_3d_main_contents(&sc3_main_cont);
        return error_code__;
    }
    error_code__ = write_speed_charm_3d_main_table(&sc3_main_cont);
    if(error_code__ < SC3_FILE_OUT_CODE) {
        scl_close(sc3_main_cont.sc3_f__);
        //delete_speed_charm_3d_main_contents(&sc3_main_cont);
        return error_code__;
    }

    error_code__ = write_speed_charm_3d_user(&sc3_main_cont, NULL);
    if(error_code__ < SC3_FILE_OUT_CODE) {
        scl_close(sc3_main_cont.sc3_f__);
        //delete_speed_charm_3d_main_contents(&sc3_main_cont);
        return error_code__;
    }
    error_code__ = write_speed_charm_3d_data_main(&sc3_main_cont);
    if(error_code__ < SC3_FILE_OUT_CODE) {
        scl_close(sc3_main_cont.sc3_f__);
        //delete_speed_charm_3d_main_contents(&sc3_main_cont);
        return error_code__;
    }
    error_code__ = write_speed_charm_3d_close(&sc3_main_cont);
    //delete_speed_charm_3d_main_contents(&sc3_main_cont);
    //scl_close(sc3_main_cont.sc3_f__);
    return error_code__;

}

sc_err_t write_speed_charm_3d_data_main(Speed_Charm_3D_Main_Contents * sc3_main_cont){
    sc_size end_pos__;
    /*after place to calc mesh first pos macro*/
    end_pos__ = scl_get_fpos(sc3_main_cont->sc3_f__);
    
    optimize_sc3_mains(sc3_main_cont);
    solution_sc3_skeleton_sort(sc3_main_cont);
    sc_err_t error_code__ = write_speed_charm_3d_mesh(sc3_main_cont, &end_pos__);

    if(error_code__ < SC3_FILE_OUT_CODE) {
        //fclose(sc3_main_cont->sc3_f__);
        //delete_speed_charm_3d_main_contents(&sc3_main_cont);
        return error_code__;
    }

    error_code__ = write_speed_charm_3d_bone(sc3_main_cont, &end_pos__);
    if(error_code__ < SC3_FILE_OUT_CODE) {
        //fclose(sc3_main_cont->sc3_f__);
        //delete_speed_charm_3d_main_contents(&sc3_main_cont);
        return error_code__;
    }
    error_code__ = write_speed_charm_3d_anim(sc3_main_cont, &end_pos__);

    if(error_code__ < SC3_FILE_OUT_CODE) {
        //fclose(sc3_main_cont->sc3_f__);
        //delete_speed_charm_3d_main_contents(&sc3_main_cont);
        return error_code__;
    }
    error_code__ = write_speed_charm_3d_tex(sc3_main_cont, &end_pos__);
    if(error_code__ < SC3_FILE_OUT_CODE) {
        //fclose(sc3_main_cont->sc3_f__);
        //delete_speed_charm_3d_main_contents(&sc3_main_cont);
        return error_code__;
    }

    return SC3_FILE_OUT_CODE;
}

sc_err_t write_speed_charm_3d_bone(Speed_Charm_3D_Main_Contents * sc3_main_cont, sc_size * end_size){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if(!end_size) return SC3_FILE_ERROR_VAR_NULL;
    //goto_sc3f_pos(sc3_main_cont->sc3_f__, *end_size);

    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;
    
    goto_sc3f_pos(sc3_main_cont->sc3_f__, *end_size);
    sc_size now_pos__ = scl_get_fpos(sc3_main_cont->sc3_f__);
    sc_size bone_size__ = 0;
    
    scl_fwrite(sc3_main_cont->sc3_f__, &bone_size__, sc_size, 1);
    bone_size__ += sizeof(sc_size) * 1;
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_BONE_CHUNK, sc_byte, len_of_str(SC3_BONE_CHUNK));
    bone_size__ += sizeof(sc_byte) * len_of_str(SC3_BONE_CHUNK);
    scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->bones_num, sc_size, 1);
    bone_size__ += sizeof(sc_size) * 1;
    scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->skeletons_num, sc_size, 1);
    bone_size__ += sizeof(sc_size) * 1;
    
    /**
     * unit mat4
     * 3,7,11,15
     * idx
     * is
     * 0 0 0 1
     * data
     * ...
     * cut
     * and
     * padding
     * but
     * call
     * fwrite
     * is
     * heavy
     * so
     * this
     * problem
     * is
     * thinking
    */
    if(!sc3_main_cont->sc3_main->sc_bones){
         if(!sc3_main_cont->sc3_main->bones_num){
            *end_size = scl_get_fpos(sc3_main_cont->sc3_f__);
            goto_sc3f_pos(sc3_main_cont->sc3_f__, now_pos__);
            scl_fwrite(sc3_main_cont->sc3_f__, &bone_size__, sc_size, 1);
            sc_size bone_table_pos__ = SC3_BONE_TABLE_POS;
            goto_sc3f_pos(sc3_main_cont->sc3_f__, bone_table_pos__);
            scl_fwrite(sc3_main_cont->sc3_f__, &now_pos__, sc_size, 1);
            return SC3_FILE_OUT_CODE;
         }
         else return SC3_FILE_ERROR_MATCH_BONE_DATA;
    }
    if(!sc3_main_cont->sc3_main->sc_skeletons){
         if(!sc3_main_cont->sc3_main->skeletons_num){
            *end_size = scl_get_fpos(sc3_main_cont->sc3_f__);
            goto_sc3f_pos(sc3_main_cont->sc3_f__, now_pos__);
            scl_fwrite(sc3_main_cont->sc3_f__, &bone_size__, sc_size, 1);
            const sc_size bone_table_pos__ = SC3_BONE_TABLE_POS;
            goto_sc3f_pos(sc3_main_cont->sc3_f__, bone_table_pos__);
            scl_fwrite(sc3_main_cont->sc3_f__, &now_pos__, sc_size, 1);
            return SC3_FILE_OUT_CODE;
         }
         else return SC3_FILE_ERROR_MATCH_SKELETON_DATA;
    }
    scl_fwrite(sc3_main_cont->sc3_f__, sc3_main_cont->sc3_main->sc_bones, Speed_Charm_3D_Bone_Map, sc3_main_cont->sc3_main->bones_num);
    bone_size__ += sizeof(Speed_Charm_3D_Bone_Map) * sc3_main_cont->sc3_main->bones_num;
    scl_fwrite(sc3_main_cont->sc3_f__, sc3_main_cont->sc3_main->sc_skeletons, Speed_Charm_3D_Skeleton, sc3_main_cont->sc3_main->skeletons_num);
    bone_size__ += sizeof(Speed_Charm_3D_Skeleton) * sc3_main_cont->sc3_main->skeletons_num;
    *end_size = scl_get_fpos(sc3_main_cont->sc3_f__);
    goto_sc3f_pos(sc3_main_cont->sc3_f__, now_pos__);
    scl_fwrite(sc3_main_cont->sc3_f__, &bone_size__, sc_size, 1);
    sc_size bone_table_pos__ = SC3_BONE_TABLE_POS;
    goto_sc3f_pos(sc3_main_cont->sc3_f__, bone_table_pos__);
    scl_fwrite(sc3_main_cont->sc3_f__, &now_pos__, sc_size, 1);
    return SC3_FILE_OUT_CODE;

}

sc_err_t write_speed_charm_3d_anim(Speed_Charm_3D_Main_Contents * sc3_main_cont, sc_size * end_size){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if(!end_size) return SC3_FILE_ERROR_VAR_NULL;

    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;

    goto_sc3f_pos(sc3_main_cont->sc3_f__, *end_size);

    //goto_sc3f_pos(sc3_main_cont->sc3_f__, *end_size);
    sc_size now_pos__ = scl_get_fpos(sc3_main_cont->sc3_f__);
    sc_size anim_size__ = 0;
    scl_fwrite(sc3_main_cont->sc3_f__, &anim_size__, sc_size, 1);
    anim_size__ += calc_type_size(sc_size, 1);
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_ANIM_CHUNK, sc_byte, len_of_str(SC3_ANIM_CHUNK));
    anim_size__ += calc_type_size(sc_byte, len_of_str(SC3_ANIM_CHUNK));
    
    scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->anim_num, sc_size, 1);
    anim_size__ += calc_type_size(sc_size, 1);

    if(!sc3_main_cont->sc3_main->sc_anim) {
        if(!sc3_main_cont->sc3_main->anim_num){
            *end_size = scl_get_fpos(sc3_main_cont->sc3_f__);
            goto_sc3f_pos(sc3_main_cont->sc3_f__, now_pos__);
            scl_fwrite(sc3_main_cont->sc3_f__, &anim_size__, sc_size, 1);
            const sc_size anim_table_pos__ = SC3_ANIM_TABLE_POS;
            goto_sc3f_pos(sc3_main_cont->sc3_f__, anim_table_pos__);
            scl_fwrite(sc3_main_cont->sc3_f__, &now_pos__, sc_size, 1);
            return SC3_FILE_OUT_CODE;
        }
        else return SC3_FILE_ERROR_ANIM;
    }
    
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_ANIM_INFORMATION_CHUNK, sc_byte, len_of_str(SC3_ANIM_INFORMATION_CHUNK));
    anim_size__ += calc_type_size(sc_byte, len_of_str(SC3_ANIM_INFORMATION_CHUNK));

    for(sc_size i = 0; i < sc3_main_cont->sc3_main->anim_num; ++i){
        scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_anim[i].frame_num, sc_size, 1);
        anim_size__ += calc_type_size(sc_size, 1);
        scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_anim[i].time_beg, float, 1);
        anim_size__ += calc_type_size(float, 1);
        scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_anim[i].time_end, float, 1);
        anim_size__ += calc_type_size(float, 1);
        scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_anim[i].framerate, float, 1);
        anim_size__ += calc_type_size(float, 1);
        if(!sc3_main_cont->sc3_main->sc_anim[i].anim_deforms){
            if(sc3_main_cont->sc3_main->skeletons_num) return SC3_FILE_ERROR_MATCH_ANIM_DEF_DATA;
        }
        sc_size opt_deform_num__ = (sc3_main_cont->sc3_main->skeletons_num + 1) / 2;
        for(sc_size j = 0; j < opt_deform_num__; ++j){
            sc_ubyte sc3_dfs__ = sc3_main_cont->sc3_main->sc_anim[i].anim_deforms[j].deform_status;
            scl_fwrite(sc3_main_cont->sc3_f__, &sc3_dfs__, sc_ubyte, 1);
        }
        anim_size__ += calc_type_size(sc_ubyte, opt_deform_num__);
    }
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_ANIM_DATA_CHUNK, sc_byte, len_of_str(SC3_ANIM_DATA_CHUNK));
    anim_size__ += calc_type_size(sc_byte, len_of_str(SC3_ANIM_DATA_CHUNK));
    //printf("%u\n", ftell(sc3_main_cont->sc3_f__));
    //sizeof(Speed_Charm_3D_Anim_Deform);

    for(sc_size i = 0; i < sc3_main_cont->sc3_main->anim_num; ++i){
        for(sc_size j = 0; j < sc3_main_cont->sc3_main->skeletons_num; ++j){
            //fwrite(&sc3_main_cont->sc3_main->sc_anim[i].anim_deforms[j].deform_status, sizeof(sc_ubyte), 1, sc3_main_cont->sc3_f__);
            //anim_size += calc_type_size(sc_ubyte, 1);
            
            sc_ubyte dfs__ = get_animation_deform_status_opt(j, &sc3_main_cont->sc3_main->sc_anim[i]);
            sc_size deforms_num__ = dfs__ & SC3_DEFORM_ROT_STATIC ? 1 : sc3_main_cont->sc3_main->sc_anim[i].frame_num;
            //printf("%f\n", sc3_main_cont->sc3_main->sc_anim[i].anim_deforms[j].rot[0]);
            scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_anim[i].anim_deforms[j].rot[0], SGG_Vector4, deforms_num__);
            anim_size__ += calc_type_size(SGG_Vector4, deforms_num__);
            deforms_num__ = dfs__ & SC3_DEFORM_POS_STATIC ? 1 : sc3_main_cont->sc3_main->sc_anim[i].frame_num;
            scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_anim[i].anim_deforms[j].pos[0], SGG_Vector3, deforms_num__);
            anim_size__ += calc_type_size(SGG_Vector3, deforms_num__);
            deforms_num__ = dfs__ & SC3_DEFORM_SCALE_STATIC ? 1 : sc3_main_cont->sc3_main->sc_anim[i].frame_num;
            scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_anim[i].anim_deforms[j].scale[0], SGG_Vector3, deforms_num__);
            anim_size__ += calc_type_size(SGG_Vector3, deforms_num__);
        }
    }

    *end_size = scl_get_fpos(sc3_main_cont->sc3_f__);
    goto_sc3f_pos(sc3_main_cont->sc3_f__, now_pos__);
    scl_fwrite(sc3_main_cont->sc3_f__, &anim_size__, sc_size, 1);
    const sc_size anim_table_pos__ = SC3_ANIM_TABLE_POS;
    goto_sc3f_pos(sc3_main_cont->sc3_f__, anim_table_pos__);
    scl_fwrite(sc3_main_cont->sc3_f__, &now_pos__, sc_size, 1);
    return SC3_FILE_OUT_CODE;

}

sc_err_t write_speed_charm_3d_tex(Speed_Charm_3D_Main_Contents * sc3_main_cont,  sc_size * end_size){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if(!end_size) return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;


    goto_sc3f_pos(sc3_main_cont->sc3_f__, *end_size);

    if(SC_POLICY_CHECK_MODULE_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_TEXTURE_VALID_VERSION)) {
        /*not implement*/

        goto_sc3f_pos(sc3_main_cont->sc3_f__, *end_size);
        sc_size now_pos = ftell(sc3_main_cont->sc3_f__);
        sc_size tex_size = 0;
        fwrite(&tex_size, sizeof(sc_size), 1, sc3_main_cont->sc3_f__);
        tex_size += sizeof(sc_size) * 1;
        if(sc3_main_cont->sc3_optional & SC3_SCI_TEXTURE_OPTIONAL){
            fwrite(SC3_SCI_IMAGE_CHUNK, sizeof(sc_byte), len_of_str(SC3_SCI_IMAGE_CHUNK), sc3_main_cont->sc3_f__);
            tex_size += sizeof(sc_byte) * len_of_str(SC3_SCI_IMAGE_CHUNK);
            
        }
        else{
            fwrite(SC3_TEXTURE_CHUNK, sizeof(sc_byte), STRING_LITERAL_SIZE(SC3_TEXTURE_CHUNK), sc3_main_cont->sc3_f__);
            tex_size += sizeof(sc_byte) * len_of_str(SC3_TEXTURE_CHUNK);

        }

        *end_size = ftell(sc3_main_cont->sc3_f__);

        goto_sc3f_pos(sc3_main_cont->sc3_f__, now_pos);

        fwrite(&tex_size, sizeof(sc_size), 1, sc3_main_cont->sc3_f__);

        sc_size size = SC3_TEX_TABLE_POS;

        goto_sc3f_pos(sc3_main_cont->sc3_f__, size);

        fwrite(&now_pos, sizeof(sc_size), 1, sc3_main_cont->sc3_f__);
    }
    return SC3_FILE_OUT_CODE;

}

sc_err_t write_speed_charm_3d_mesh(Speed_Charm_3D_Main_Contents * sc3_main_cont, sc_size * end_size){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if(!end_size) return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;

    /**
     * [size]
     * [chunk]
     * [raw data num]
     * [raw data inf chunk]
     * [raw data inf]
     * [raw data chunk]
     * [raw data]
    */
   
    goto_sc3f_pos(sc3_main_cont->sc3_f__, *end_size);
    sc_size now_pos__ = scl_get_fpos(sc3_main_cont->sc3_f__);
    sc_size mesh_size__ = 0;
    
    scl_fwrite(sc3_main_cont->sc3_f__, &mesh_size__, sc_size, 1);
    
    mesh_size__ += sizeof(sc_size) * 1;
    
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_MESH_CHUNK, sc_byte, len_of_str(SC3_MESH_CHUNK));
    mesh_size__ += sizeof(sc_byte) * len_of_str(SC3_MESH_CHUNK);

    if(!sc3_main_cont->sc3_main->sc_meshes) {
        if(!sc3_main_cont->sc3_main->meshes_num){
            *end_size = scl_get_fpos(sc3_main_cont->sc3_f__);
            const sc_size mesh_table_pos__ = SC3_MESH_TABLE_POS;
            goto_sc3f_pos(sc3_main_cont->sc3_f__, mesh_table_pos__);
            scl_fwrite(sc3_main_cont->sc3_f__, &now_pos__, sc_size, 1);
            goto_sc3f_pos(sc3_main_cont->sc3_f__, now_pos__);
            scl_fwrite(sc3_main_cont->sc3_f__, &mesh_size__, sc_size, 1);
            //printf("size%lu %lu %lu\n", mesh_size__, now_pos, *end_size);
            return SC3_FILE_OUT_CODE;
        }
        return SC3_FILE_ERROR_MESH;
    }
    scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->meshes_num, sc_size, 1);
    mesh_size__ += sizeof(sc_size) * 1;
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_MESH_INFORMATION_CHUNK, sc_byte, len_of_str(SC3_MESH_INFORMATION_CHUNK));
    mesh_size__ += sizeof(sc_byte) * len_of_str(SC3_MESH_INFORMATION_CHUNK);
    for(sc_size i__ = 0; i__ < sc3_main_cont->sc3_main->meshes_num; ++i__){
        Speed_Charm_3D_Mesh * sc3_mesh = sc3_main_cont->sc3_main->sc_meshes + i__;           
        scl_fwrite(sc3_main_cont->sc3_f__, &sc3_mesh->attr_num, sc_size, 1);
        mesh_size__ += sizeof(sc_size) * 1;
        scl_fwrite(sc3_main_cont->sc3_f__, &sc3_mesh->ind_num, sc_size, 1);
        mesh_size__ += sizeof(sc_size) * 1;
        /**
         * (0 + 7) / 8 = 0
         * (1 + 7) / 8 = 1
         * (2 + 7) / 8 = 
        */
        if(i__ < (sc3_main_cont->sc3_main->meshes_num  + SC3_OFFSET_INF_NUM) / SC_POLICY_SC_UINT_8_BITS){
            scl_fwrite(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->sc_meshes[i__].mesh_inf, sc_ubyte, 1);
            mesh_size__ += sizeof(sc_ubyte) * 1;
        }
    }
    
    scl_fwrite(sc3_main_cont->sc3_f__, SC3_MESH_DATA_CHUNK, sc_byte, len_of_str(SC3_MESH_DATA_CHUNK));
    mesh_size__ += sizeof(sc_byte) * len_of_str(SC3_MESH_DATA_CHUNK);
    for(sc_size i = 0; i < sc3_main_cont->sc3_main->meshes_num; ++i){
        Speed_Charm_3D_Mesh * sc3_mesh__ = sc3_main_cont->sc3_main->sc_meshes + i;
        if(!sc3_mesh__->mesh_attr) 
            if(sc3_mesh__->attr_num) return SC3_FILE_ERROR_MATCH_MESH_VERT_DATA;
        scl_fwrite(sc3_main_cont->sc3_f__, sc3_mesh__->mesh_attr, Speed_Charm_3D_Mesh_Attr, sc3_mesh__->attr_num);
        mesh_size__ += sizeof(Speed_Charm_3D_Mesh_Attr) * sc3_mesh__->attr_num;
        sc_ubyte idx__ = i / SC_POLICY_SC_UINT_8_BITS;
        sc_ubyte inf__ = (*(sc3_main_cont->sc3_main->sc_meshes + idx__)).mesh_inf >> (i % SC_POLICY_SC_UINT_8_BITS);
        if(inf__ & SC3_MAIN_HAVE_SKIN) {
            if(!sc3_mesh__->skin_attr) 
                if(sc3_mesh__->attr_num) return SC3_FILE_ERROR_MATCH_MESH_SKIN_DATA;

            scl_fwrite(sc3_main_cont->sc3_f__, sc3_mesh__->skin_attr, Speed_Charm_3D_Skin_Attr, sc3_mesh__->attr_num);
            mesh_size__ += sizeof(Speed_Charm_3D_Skin_Attr) * sc3_mesh__->attr_num;
            //scl_fwrite(sc3_main_cont->sc3_f__, &sc3_mesh->mesh_bone_map_num, sc_size, 1);
            //mesh_size__ += sizeof(sc_size);
            //scl_fwrite(sc3_main_cont->sc3_f__, sc3_mesh->mesh_bone_map, Speed_Charm_3D_Mesh_Bone_Map, sc3_mesh->mesh_bone_map_num);
            //mesh_size__ += sizeof(Speed_Charm_3D_Mesh_Bone_Map) * sc3_mesh->mesh_bone_map_num;
        }

        scl_fwrite(sc3_main_cont->sc3_f__, sc3_mesh__->indices, sc_uint, sc3_mesh__->ind_num);
        mesh_size__ += sizeof(sc_uint) * sc3_mesh__->ind_num;

    }

    *end_size = scl_get_fpos(sc3_main_cont->sc3_f__);
    const sc_size mesh_table_pos__ = SC3_MESH_TABLE_POS;
    goto_sc3f_pos(sc3_main_cont->sc3_f__, mesh_table_pos__);
    scl_fwrite(sc3_main_cont->sc3_f__, &now_pos__, sc_size, 1);
    goto_sc3f_pos(sc3_main_cont->sc3_f__, now_pos__);
    scl_fwrite(sc3_main_cont->sc3_f__, &mesh_size__, sc_size, 1);
    //printf("size%lu %lu %lu\n", mesh_size__, now_pos, *end_size);
    return SC3_FILE_OUT_CODE;
}

sc_err_t read_speed_charm_3d_main(Speed_Charm_3D_Main ** sc3_main, const sc_byte * sc3_path){
    if(!sc3_main) return SC3_FILE_ERROR_VAR_NULL;
    Speed_Charm_3D_Main_Contents * sc3_main_cont__;
    sc3_main_cont__ = init_speed_charm_3d_main_contents(NULL);
    Speed_Charm_3D_Tags sc3_tags;
    sc_err_t error_code__ = read_speed_charm_3d_entry_data(sc3_main_cont__, &sc3_tags, sc3_path);
    if(SC_POLICY_ERROR_CHECK(error_code__)) {
        //scl_close(sc3_main_cont->sc3_f__);
        delete_speed_charm_3d_main_contents(&sc3_main_cont__);
        return error_code__;
    }

    error_code__ = read_speed_charm_3d_table(sc3_main_cont__, &sc3_tags);
    if(SC_POLICY_ERROR_CHECK(error_code__)) {
        scl_close(sc3_main_cont__->sc3_f__);
        delete_speed_charm_3d_main_contents(&sc3_main_cont__);
        return error_code__;
    }
    error_code__ = read_speed_charm_3d_user(sc3_main_cont__, &sc3_tags);
    if(SC_POLICY_ERROR_CHECK(error_code__)) {
        scl_close(sc3_main_cont__->sc3_f__);
        delete_speed_charm_3d_main_contents(&sc3_main_cont__);
        return error_code__;
    }

    error_code__ = read_speed_charm_3d_data(sc3_main_cont__, &sc3_tags);

    if(SC_POLICY_ERROR_CHECK(error_code__)) {
        
        scl_close(sc3_main_cont__->sc3_f__);
        delete_speed_charm_3d_main_contents(&sc3_main_cont__);
        return error_code__;
    }

    error_code__ = read_speed_charm_3d_close(sc3_main_cont__, &sc3_tags);
    if(SC_POLICY_ERROR_CHECK(error_code__)) {
        delete_speed_charm_3d_main_contents(&sc3_main_cont__);
        return error_code__;
    }

    *sc3_main = sc3_main_cont__->sc3_main;
    sc3_main_cont__->sc3_main = NULL;
    delete_speed_charm_3d_main_contents(&sc3_main_cont__);

    return SC3_FILE_OUT_CODE;
}

sc_err_t read_speed_charm_3d_entry_data(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags, const sc_byte * sc3_path){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_tags)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_path)return SC3_FILE_ERROR_VAR_NULL;
    sc_byte sc3_file_path__[SC3_FILE_NAME_DEFAULT_SIZE] = {0};
    if(!ext_str_cpy(sc3_file_path__, sc3_path, SC3_FILE_NAME_DEFAULT_SIZE, safety_strlen(sc3_path) + 1)) return SC3_FILE_ERROR_VAR_NULL;
    set_sc3_file_format(sc3_file_path__);
    if(scl_fopen(sc3_main_cont->sc3_f__, sc3_file_path__, "rb")) return SC3_FILE_R_FOPEN;
    if(!smart_sc3_tags_reads(sc3_tags->sc3_entry_magic_num, SC3_FILE_ENTRY_MAGIC_NUMBER, len_of_str(SC3_FILE_ENTRY_MAGIC_NUMBER), sc3_main_cont->sc3_f__)) return SC3_FILE_READ_ENTRY_MAGIC_NUM;
    scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_version, sc_ushort, 1);   
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;
    if(!smart_sc3_tags_reads(sc3_tags->sc3_beg, SC_POLICY_REQUIRE_BEG_CHUNK, len_of_str(SC_POLICY_REQUIRE_BEG_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_READ_BEG;
    if(!smart_sc3_tags_reads(sc3_tags->sc3_tbl, SC3_MAIN_TABLE, len_of_str(SC3_MAIN_TABLE), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_MAIN_TABLE;
        //fwrite(SC3_MAIN_BEG, sizeof(sc_byte), STRING_LITERAL_SIZE(SC3_MAIN_BEG), sc3_main_cont->sc3_f__);
        //fwrite(SC3_MAIN_TABLE, sizeof(sc_byte), STRING_LITERAL_SIZE(SC3_MAIN_TABLE), sc3_main_cont->sc3_f__);
    return SC3_FILE_OUT_CODE;
    
}

sc_err_t read_speed_charm_3d_table(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags){
    if(!sc3_main_cont || !sc3_tags)return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC_POLICY_ERROR_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;
    if(!smart_sc3_tags_reads(sc3_tags->sc3_tbl, SC3_MAIN_TABLE_MESH, len_of_str(SC3_MAIN_TABLE_MESH), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_MESH_TABLE;
    scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->mesh_pos, sizeof(sc_byte), 1);
    //fwrite(SC3_MAIN_TABLE_MESH, sizeof(sc_byte), STRING_LITERAL_SIZE(SC3_MAIN_TABLE_MESH), sc3_main_cont->sc3_f__);
    //fwrite(&booking_table_pos, sizeof(sc_size), 1, sc3_main_cont->sc3_f__);
    if(!smart_sc3_tags_reads(sc3_tags->sc3_tbl, SC3_MAIN_TABLE_BONE, len_of_str(SC3_MAIN_TABLE_BONE), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_BONE_TABLE;
    scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->bone_pos, sizeof(sc_byte), 1);
    if(!smart_sc3_tags_reads(sc3_tags->sc3_tbl, SC3_MAIN_TABLE_ANIM, len_of_str(SC3_MAIN_TABLE_ANIM), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_BONE_TABLE;
    scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->anim_pos, sizeof(sc_byte), 1);

    //if(smart_sc3_tags_reads(sc3_tags->sc3_tbl, SC3_MAIN_TABLE_ANIM, STRING_LITERAL_SIZE(SC3_MAIN_TABLE_TEXTURE), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_BONE_TABLE;
    //scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->anim_pos, sizeof(sc_byte), 1);
    if(sc3_main_cont->sc3_version >= SC3_TEXTURE_VALID_VERSION){
        if(!smart_sc3_tags_reads(sc3_tags->sc3_tbl, SC3_MAIN_TABLE_ANIM, len_of_str(SC3_MAIN_TABLE_TEXTURE), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_TEX_TABLE;
        scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->tex_pos, sizeof(sc_byte), 1);
    }
    return SC3_FILE_OUT_CODE;

}

sc_err_t read_speed_charm_3d_user(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_tags)return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC_POLICY_ERROR_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;
    if(!smart_sc3_tags_reads(sc3_tags->sc3_usr, SC3_MAIN_USER, len_of_str(SC3_MAIN_USER), sc3_main_cont->sc3_f__)) return SC3_FILE_READ_USR;
    if(!smart_sc3_tags_reads(sc3_tags->sc3_usr, SC3_MAIN_COMMENT, STRING_LITERAL_SIZE(SC3_MAIN_COMMENT), sc3_main_cont->sc3_f__)) return SC3_FILE_READ_COM;
    scl_fread(sc3_main_cont->sc3_f__, sc3_tags->sc3_com, sc_byte, SC3_COMMENT_SIZE_MAX);
    sc_byte vd_data__[SC3_FREE_DATA_SIZE] = {0};
    if(!smart_sc3_tags_reads(sc3_tags->sc3_usr, SC3_FREE_DATA_CHUNK, len_of_str(SC3_FREE_DATA_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_VIRTUAL_DATA;
    scl_fread(sc3_main_cont->sc3_f__, vd_data__, sc_byte, SC3_FREE_DATA_SIZE);
    return SC3_FILE_OUT_CODE;
}

sc_err_t read_speed_charm_3d_close(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main){
        scl_close(sc3_main_cont->sc3_f__);
        return SC3_FILE_ERROR_VAR_NULL;
    }
    if(!sc3_tags) {
        scl_close(sc3_main_cont->sc3_f__);
        return SC3_FILE_ERROR_VAR_NULL;
    }
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) {
        scl_close(sc3_main_cont->sc3_f__);
        return SC3_INVALID_VERSION;
    }
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) {
        scl_close(sc3_main_cont->sc3_f__);
        return SC3_INVALID_VERSION;
    }
    fseek(sc3_main_cont->sc3_f__, 0, SEEK_END);
    fseek(sc3_main_cont->sc3_f__, -len_of_str(SC3_MAIN_END), SEEK_CUR);

    if(!smart_sc3_tags_reads(sc3_tags->sc3_end, SC3_MAIN_END, len_of_str(SC3_MAIN_END), sc3_main_cont->sc3_f__)) return SC3_FILE_READ_END;
    //printf("debug out ss %u\n", ftell(sc3_main_cont->sc3_f__));

    //printf("debug out ss\n");
    if(!SC_POLICY_REQUIRE_CHECK_END_NEXT(fgetc(sc3_main_cont->sc3_f__))) {
        scl_close(sc3_main_cont->sc3_f__);
        return SC3_FILE_ERROR_FILE_END_NEXT;
    }
    scl_close(sc3_main_cont->sc3_f__);
    return SC3_FILE_OUT_CODE;
}
sc_err_t read_speed_charm_3d_data(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags){ 
    sc_err_t error_code__ = read_speed_charm_3d_mesh(sc3_main_cont, sc3_tags);

    if(SC_POLICY_ERROR_CHECK(error_code__)) return error_code__;

    error_code__ = read_speed_charm_3d_bone(sc3_main_cont, sc3_tags);
                //printf("debug out %i %u\n", 1, error_code);    
    if(SC_POLICY_ERROR_CHECK(error_code__)) return error_code__;


    error_code__ = read_speed_charm_3d_anim(sc3_main_cont, sc3_tags);
    if(SC_POLICY_ERROR_CHECK(error_code__)) return error_code__;

    error_code__ = read_speed_charm_3d_tex(sc3_main_cont, sc3_tags);
    
    return SC3_FILE_OUT_CODE;
}
sc_err_t read_speed_charm_3d_tex(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_tags) return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;

    if(!SC_POLICY_CHECK_MODULE_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_TEXTURE_VALID_VERSION)) return SC3_INVALID_VERSION;
    goto_sc3f_pos(sc3_main_cont->sc3_f__, sc3_main_cont->tex_pos);
    return SC3_FILE_OUT_CODE;
}

sc_err_t read_speed_charm_3d_bone(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags){
    
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_tags) return SC3_FILE_ERROR_VAR_NULL;

    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;

    goto_sc3f_pos(sc3_main_cont->sc3_f__, sc3_main_cont->bone_pos);

    sc_size sum_check_size__ = 0;
    scl_fread(sc3_main_cont->sc3_f__, &sum_check_size__, sc_size, 1);

    if(!smart_sc3_tags_reads(sc3_tags->sc3_chunks, SC3_BONE_CHUNK, len_of_str(SC3_BONE_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_BONE;
    scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->bones_num, sc_size, 1);
    sc3_main_cont->sc3_main->sc_bones = smart_malloc(Speed_Charm_3D_Bone_Map, sc3_main_cont->sc3_main->bones_num);
    
    scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->skeletons_num, sc_size, 1);
    sc3_main_cont->sc3_main->sc_skeletons = smart_malloc(Speed_Charm_3D_Skeleton, sc3_main_cont->sc3_main->skeletons_num);

    if(!sc3_main_cont->sc3_main->sc_bones) return SC_POLICY_ERROR_ALLOC;
    if(!sc3_main_cont->sc3_main->sc_skeletons) return SC_POLICY_ERROR_ALLOC;
    
    scl_fread(sc3_main_cont->sc3_f__, sc3_main_cont->sc3_main->sc_bones, Speed_Charm_3D_Bone_Map, sc3_main_cont->sc3_main->bones_num);
    scl_fread(sc3_main_cont->sc3_f__, sc3_main_cont->sc3_main->sc_skeletons, Speed_Charm_3D_Skeleton, sc3_main_cont->sc3_main->skeletons_num);
    
    sc_size end_pos__ = scl_get_fpos(sc3_main_cont->sc3_f__);
                //printf("debug out %i %u\n", 1, 1);    
    //printf("debug out %i %u %u\n", end_pos, sum_check_size__, sc3_main_cont->bone_pos);    

    if(sum_check_size__ != end_pos__ - sc3_main_cont->bone_pos) return SC3_FILE_ERROR_SUM_CHECK;

    return SC3_FILE_OUT_CODE;

}

sc_err_t read_speed_charm_3d_anim(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_tags) return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;
    goto_sc3f_pos(sc3_main_cont->sc3_f__, sc3_main_cont->anim_pos);

    sc_size sum_check_size__ = 0;
    
    scl_fread(sc3_main_cont->sc3_f__, &sum_check_size__, sc_size, 1);
    
    if(!smart_sc3_tags_reads(sc3_tags->sc3_chunks, SC3_ANIM_CHUNK, len_of_str(SC3_ANIM_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_ANIM;
    
    scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->anim_num, sc_size, 1);
    
    Speed_Charm_3D_Anim * new_sc3_anim__ = smart_malloc(Speed_Charm_3D_Anim, sc3_main_cont->sc3_main->anim_num);
    
    if(!new_sc3_anim__) return SC_POLICY_ERROR_ALLOC;
    
    for(sc_size i = 0; i < sc3_main_cont->sc3_main->anim_num; ++i) new_sc3_anim__[i].anim_deforms = NULL;

    sc3_main_cont->sc3_main->sc_anim = new_sc3_anim__;
    
    if(!smart_sc3_tags_reads(sc3_tags->sc3_chunks, SC3_ANIM_INFORMATION_CHUNK, len_of_str(SC3_ANIM_INFORMATION_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_ANIM_INFORMATION;
    
    for(sc_size i = 0; i < sc3_main_cont->sc3_main->anim_num; ++i){
        Speed_Charm_3D_Anim * sc3_anim__ = sc3_main_cont->sc3_main->sc_anim + i;
        scl_fread(sc3_main_cont->sc3_f__, &sc3_anim__->frame_num, sc_size, 1);
        if(sc3_anim__->frame_num >= SC3_FRAMERATE_60) return SC3_ANIM_INFORMATION_ERROR_FRAME;
        scl_fread(sc3_main_cont->sc3_f__, &sc3_anim__->time_beg, float, 1);
        
        scl_fread(sc3_main_cont->sc3_f__, &sc3_anim__->time_end, float, 1);
        
        scl_fread(sc3_main_cont->sc3_f__, &sc3_anim__->framerate, float, 1);
        
        if(sc3_anim__->framerate >= SC3_MAX_FRAME_NUM) return SC3_ANIM_INFORMATION_ERROR_FRAMERATE;

        sc3_anim__->anim_deforms = smart_calloc(Speed_Charm_3D_Anim_Deform, sc3_main_cont->sc3_main->skeletons_num);
        
        if(!sc3_anim__->anim_deforms) return SC_POLICY_ERROR_ALLOC;
        
        sc_size opt_size__ = (sc3_main_cont->sc3_main->skeletons_num + 1) / 2;

        sc_ubyte * deform_cache__ = smart_malloc(sc_ubyte, opt_size__);
        if(!deform_cache__) return SC_POLICY_ERROR_ALLOC;

        scl_fread(sc3_main_cont->sc3_f__, deform_cache__, sc_ubyte, opt_size__);
        
        for(sc_size j = 0; j < opt_size__; ++j){
            sc_size dst__ = j * 2;
            Speed_Charm_3D_Anim_Deform * sc3_anim_def__ = sc3_anim__->anim_deforms + dst__;
            sc3_anim_def__->deform_status = deform_cache__[j] & (~0u >> SC3_HALF_UINT_8_BITS);
            if(dst__ + 1 >= sc3_main_cont->sc3_main->skeletons_num) break;
            sc3_anim_def__ += 1;
            sc3_anim_def__->deform_status = (deform_cache__[j] & (~0u << SC3_HALF_UINT_8_BITS)) >> SC3_HALF_UINT_8_BITS;
        }
        s_free(deform_cache__);
    }
    
    if(!smart_sc3_tags_reads(sc3_tags->sc3_chunks, SC3_ANIM_DATA_CHUNK, len_of_str(SC3_ANIM_DATA_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_ANIM_DATA;
    for(sc_size i = 0; i < sc3_main_cont->sc3_main->anim_num; ++i){
        Speed_Charm_3D_Anim * sc3_anim__ = sc3_main_cont->sc3_main->sc_anim + i;
        for(sc_size j = 0; j < sc3_main_cont->sc3_main->skeletons_num; ++j){
            Speed_Charm_3D_Anim_Deform * sc3_anim_def__ = sc3_anim__->anim_deforms + j;
            //scl_fread(sc3_main_cont->sc3_f__,&sc3_main_cont->sc3_main->sc_anim[i].anim_deforms[j].deform_status, sc_ubyte, 1);
            sc_size deforms_num__ = sc3_anim_def__->deform_status & SC3_DEFORM_ROT_STATIC ? 1 : sc3_anim__->frame_num;
            sc3_anim_def__->rot = smart_malloc(SGG_Vector4, deforms_num__);
            if(!sc3_anim_def__->rot) return SC_POLICY_ERROR_ALLOC;
            scl_fread(sc3_main_cont->sc3_f__, sc3_anim_def__->rot, SGG_Vector4, deforms_num__);
            
            deforms_num__ = sc3_anim_def__->deform_status & SC3_DEFORM_POS_STATIC ? 1 : sc3_anim__->frame_num;
            sc3_anim_def__->pos = smart_malloc(SGG_Vector3, deforms_num__);
            if(!sc3_anim_def__->pos) return SC_POLICY_ERROR_ALLOC;
            
            scl_fread(sc3_main_cont->sc3_f__, sc3_anim_def__->pos, SGG_Vector3, deforms_num__);
            deforms_num__ = sc3_anim_def__->deform_status & SC3_DEFORM_SCALE_STATIC ? 1 : sc3_anim__->frame_num;
            sc3_anim_def__->scale = smart_malloc(SGG_Vector3, deforms_num__);
            if(!sc3_anim_def__->scale) return SC_POLICY_ERROR_ALLOC;
            scl_fread(sc3_main_cont->sc3_f__, sc3_anim_def__->scale, SGG_Vector3, deforms_num__);
        }
    }
    sc_size end_pos__ = scl_get_fpos(sc3_main_cont->sc3_f__);

    if(sum_check_size__ != end_pos__ - sc3_main_cont->anim_pos) return SC3_FILE_ERROR_SUM_CHECK;
    return SC3_FILE_OUT_CODE;

}

sc_err_t read_speed_charm_3d_mesh(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags){
    if(!sc3_main_cont)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_main_cont->sc3_main)return SC3_FILE_ERROR_VAR_NULL;
    if(!sc3_tags) return SC3_FILE_ERROR_VAR_NULL;
    if(!SC_POLICY_CHECK_BASE_VERSION(sc3_main_cont->sc3_version)) return SC3_INVALID_VERSION;
    if(!SC_POLICY_CHECK_SUPPORT_VERSION(sc3_main_cont->sc3_version, SC3_NOW_VERSION)) return SC3_INVALID_VERSION;
    
    /**
     * Search for table pos.
    */
    goto_sc3f_pos(sc3_main_cont->sc3_f__, sc3_main_cont->mesh_pos);
    sc_size sum_check_mesh_size__;
    scl_fread(sc3_main_cont->sc3_f__, &sum_check_mesh_size__, sc_size, 1);

    if(!smart_sc3_tags_reads(sc3_tags->sc3_chunks, sc3_tags->sc3_chunks, len_of_str(SC3_MESH_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_MESH;

    scl_fread(sc3_main_cont->sc3_f__, &sc3_main_cont->sc3_main->meshes_num, sc_size, 1);

    sc3_main_cont->sc3_main->sc_meshes = smart_calloc(Speed_Charm_3D_Mesh, sc3_main_cont->sc3_main->meshes_num);

    if(!sc3_main_cont->sc3_main->sc_meshes) return SC3_FILE_ERROR_MESH;

    if(!smart_sc3_tags_reads(sc3_tags->sc3_chunks, sc3_tags->sc3_chunks, len_of_str(SC3_MESH_INFORMATION_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_MESH_INFORMATION;
    for(sc_size i = 0; i < sc3_main_cont->sc3_main->meshes_num; ++i){
        Speed_Charm_3D_Mesh * sc3_mesh__ = sc3_main_cont->sc3_main->sc_meshes + i;
        scl_fread(sc3_main_cont->sc3_f__, &sc3_mesh__->attr_num, sc_size, 1);
        scl_fread(sc3_main_cont->sc3_f__, &sc3_mesh__->ind_num, sc_size, 1);
        if(i < (sc3_main_cont->sc3_main->meshes_num + SC3_OFFSET_INF_NUM) / SC_POLICY_SC_UINT_8_BITS) {
            sc_ubyte tmp_inf__;
            scl_fread(sc3_main_cont->sc3_f__, &tmp_inf__, sc_ubyte, 1);
            for(sc_size j = 0; j < SC_POLICY_SC_UINT_8_BITS; ++j){
                sc_ubyte emp__ = 1 << j;
                sc_ubyte inf__ = (emp__ & tmp_inf__) >> j;
                if(i * SC_POLICY_SC_UINT_8_BITS + j >= sc3_main_cont->sc3_main->meshes_num) break;
                (*(sc3_main_cont->sc3_main->sc_meshes + i * SC_POLICY_SC_UINT_8_BITS + j)).mesh_inf = inf__;
            }

        }
    }
    if(!smart_sc3_tags_reads(sc3_tags->sc3_chunks, sc3_tags->sc3_chunks, len_of_str(SC3_MESH_DATA_CHUNK), sc3_main_cont->sc3_f__)) return SC3_FILE_ERROR_MESH_DATA;

    for(sc_size i = 0; i < sc3_main_cont->sc3_main->meshes_num; ++i){
        
        Speed_Charm_3D_Mesh * sc3_mesh__ = sc3_main_cont->sc3_main->sc_meshes + i;
        Speed_Charm_3D_Mesh_Attr * mesh_attr__ = smart_malloc(Speed_Charm_3D_Mesh_Attr, sc3_mesh__->attr_num);
        if(!mesh_attr__) return SC_POLICY_ERROR_ALLOC;
        sc3_mesh__->mesh_attr = mesh_attr__;
        scl_fread(sc3_main_cont->sc3_f__, sc3_mesh__->mesh_attr, Speed_Charm_3D_Mesh_Attr, sc3_mesh__->attr_num);
        if(sc3_mesh__->mesh_inf & SC3_MAIN_HAVE_SKIN) {
            Speed_Charm_3D_Skin_Attr * skin_attr__ =  smart_malloc(Speed_Charm_3D_Skin_Attr, sc3_mesh__->attr_num);
            
            if(!skin_attr__) return SC_POLICY_ERROR_ALLOC;
            sc3_mesh__->skin_attr = skin_attr__;
            scl_fread(sc3_main_cont->sc3_f__, sc3_mesh__->skin_attr, Speed_Charm_3D_Skin_Attr, sc3_mesh__->attr_num);
            
            //scl_fread(sc3_main_cont->sc3_f__, &sc3_mesh->mesh_bone_map_num, sc_size, 1);
            //sc3_mesh->mesh_bone_map = smart_malloc(Speed_Charm_3D_Mesh_Bone_Map, sc3_mesh->mesh_bone_map_num);
            //if(!sc3_mesh->mesh_bone_map) return SC_POLICY_ERROR_ALLOC;

            //scl_fread(sc3_main_cont->sc3_f__, sc3_mesh->mesh_bone_map, Speed_Charm_3D_Mesh_Bone_Map, sc3_me

        }
        sc_uint * indices__ = smart_malloc(sc_uint, sc3_mesh__->ind_num);
        if(!indices__) return SC_POLICY_ERROR_ALLOC;
        sc3_mesh__->indices = indices__;
        scl_fread(sc3_main_cont->sc3_f__, sc3_mesh__->indices, sc_uint, sc3_mesh__->ind_num);
    }
    sc_size end_pos__ = scl_get_fpos(sc3_main_cont->sc3_f__);

    if(sum_check_mesh_size__ != end_pos__ - sc3_main_cont->mesh_pos) return SC3_FILE_ERROR_SUM_CHECK;

    return SC3_FILE_OUT_CODE;
}

void start_animation_speed_charm_3d_bone(Speed_Charm_3D_Run_Main * sc3_main, sc_uint anim_num, float delta_time){
    if(!sc3_main->sc_anim) return;
    const Speed_Charm_3D_Anim *sc3_anim__ = &sc3_main->sc_anim[anim_num >= sc3_main->anim_num ? 0 : anim_num];
    sc3_main->anim_times += delta_time;

    if (sc3_main->anim_times >= sc3_anim__->time_end) sc3_main->anim_times -= sc3_anim__->time_end - sc3_anim__->time_beg;

   	float frame_time__ = (sc3_main->anim_times - sc3_anim__->time_beg) * sc3_anim__->framerate;
	size_t f0__ = get_minu((size_t)frame_time__ + 0, sc3_anim__->frame_num - 1);
	size_t f1__ = get_minu((size_t)frame_time__ + 1, sc3_anim__->frame_num - 1);
	float t__ = get_minf(frame_time__ - (float)f0__, 1.0f);
    for (size_t i = 0; i < sc3_main->skeletons_num; ++i) {
        const Speed_Charm_3D_Anim_Deform *anim_deform = sc3_anim__->anim_deforms + i;
        sc_byte dfs__ = get_animation_deform_status(i, sc3_anim__);
        SGG_Vector4 rot__;
        if(SC3_CHECK_DYNAMIC_ROT(dfs__)) sgg_lerp_vector4(&rot__, anim_deform->rot + f0__, anim_deform->rot + f1__, t__);
        else rot__ = *anim_deform->rot;
        
        SGG_Vector3 pos__;
        
        if(SC3_CHECK_DYNAMIC_POS(dfs__)) sgg_lerp_vector3(&pos__, anim_deform->pos + f0__, anim_deform->pos + f1__, t__);
        else pos__ = *anim_deform->pos;
        
        SGG_Vector3 scale__;

        if(SC3_CHECK_DYNAMIC_SCL(dfs__)) sgg_lerp_vector3(&scale__, anim_deform->scale + f0__, anim_deform->scale + f1__, t__);
        else scale__ = *anim_deform->scale;
        sgg_trs_mat4(&(*(sc3_main->sc_skeletons + i)).node.to_parent, &rot__, &pos__, &scale__);
    }
}

void start_speed_charm_3d_bones_hierarchy(Speed_Charm_3D_Run_Main * sc3_main){
    for (size_t i = 0; i < sc3_main->skeletons_num; i++) {
	    Speed_Charm_3D_Skeleton *sc3_skl_map__ = &sc3_main->sc_skeletons[i];
	    if (sc3_skl_map__->parent >= 0) mul_mat4_mat4(&sc3_skl_map__->node.to_world, &(*(sc3_main->sc_skeletons + sc3_skl_map__->parent)).node.to_world, &sc3_skl_map__->node.to_parent);
        else sc3_skl_map__->node.to_world = sc3_skl_map__->node.to_parent;
    }
}

void start_build_bones_speed_charm_3d(Speed_Charm_3D_Run_Main * sc3_main, Speed_Charm_3D_Bone_Map * sc3_mesh_bone_map, SGG_Mat4 * sc3_mat4){
    Speed_Charm_3D_Skeleton * bone_node__ = sc3_main->sc_skeletons + sc3_mesh_bone_map->ind;
    mul_mat4_mat4(sc3_mat4, &bone_node__->node.to_world, &sc3_mesh_bone_map->inverse_mat);
}
