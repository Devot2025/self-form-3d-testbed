#ifndef _TEST_GAME_1_CONTENTS_MAIN_H_
#define _TEST_GAME_1_CONTENTS_MAIN_H_
#include <test_game_1_optional_main.hpp>
typedef enum Main_Shader_Var_Id{
    E_Main_Shader_Projection,
    E_Main_Shader_Model,
    E_Main_Shader_View_Pos,
    E_Main_Shader_Mesh_Pos,
    E_Main_Shader_Mesh_Color,
    E_Main_Shader_Light_Pos,
    E_Main_Shader_Light_Color,
    E_Main_Shader_Skin_Bones,
    E_Main_Shader_Static_Check,
    E_Main_Shader_Frag_Check,
    E_Main_Shader_Shadow_Map,
    E_Main_Shader_Light_View,
}Main_Shader_Var_Id;
class Test_Game_1_Main_Loop : public GL_Loop_Manager {
public:

    Emulate_Uniform_2D_Data<E_Main_Shader_Skin_Bones> emulated_bone;
    Cursor_Manager cursor_manager;
    Program_Shader_Operator main_shader_manager;
    Object_Color_Manager<E_Main_Shader_Mesh_Color> obj_color;
    Object_Light_3D<E_Main_Shader_Light_Color, E_Main_Shader_Light_Pos> sample_light;
    
    std::vector<Basic_Gl_Vertex_Package<3, GL_DYNAMIC_DRAW>> bsc3;
    
    Test_Game_1_Main_Loop();
    int init_loop() override;
    int main_loop() override;
    int end_loop() override;
};

#endif /* TEST_GAME_1_CONTENTS_MAIN */
