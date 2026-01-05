#ifndef _SC3_H_
#define _SC3_H_
#include <ggmath/sgg_vec.h>
#include <prog_c/std_ext/ext_alloc.h>
#include <prog_c/std_ext/ext_os_strs.h>
#include <c_header_helper.hint>
#include <prog_c/sc_data_policy/speed_charm_policy.h>

#include <prog_c/std_ext/ext_strs.h>


#define DEBUG_VERSION 1
#define RELEASE_VERSION 0
#define speed_charm_3d_si_strap sc_policy_si_strap

#define TOP_PARENT -1

#define DIMENSION_XYZ 3

#define SC3_FRAMERATE__ 30.0f
#define SC3_FRAMERATE_60 SC3_FRAMERATE__ * 2.0f
#define SC3_MAX_FRAME_NUM 4096
#define SC3_FILE_FORMAT SC_POLICY_FILE_FORMAT_NAME(3)

#define SC3_DEFAULT_FILE_NAME "a" SC3_FILE_FORMAT

#define SC3_MAIN_BEG SC_POLICY_REQUIRE_BEG_CHUNK

#define SC3_MAIN_END SC_POLICY_REQUIRE_END_CHUNK

#define SC3_FILE_ENTRY_MAGIC_NUMBER SC_POLICY_REQUIRE_SC_ENTRY_MAGIC_NUM("SC3")
#define SC3_MAIN_TABLE "TBL"
#define SC3_MAIN_TABLE_MESH "MES"
#define SC3_MAIN_TABLE_BONE "BON"
#define SC3_MAIN_TABLE_ANIM "ANI"
#define SC3_MAIN_TABLE_TEXTURE "TEX"

#define SC3_MESH_CHUNK "MSC"
#define SC3_MESH_INFORMATION_CHUNK "MIC"
#define SC3_MESH_DATA_CHUNK "MDC"

#define SC3_ANIM_CHUNK "ANC"
#define SC3_ANIM_INFORMATION_CHUNK "AIC"
#define SC3_ANIM_DATA_CHUNK "ADC"

#define SC3_BONE_CHUNK "BNC"

#define SC3_TEXTURE_CHUNK "TXC"

#define SC3_SCI_IMAGE_CHUNK "STC"

#define SC3_FREE_DATA_CHUNK "SFD"

#define SC3_SCI_STATIC_IMAGE_CHUNK "STC"

#define SC3_SCI_DYNAMIC_IMAGE_CHUNK "DTC"

#define SC3_MAIN_USER "USR"

#define SC3_MAIN_COMMENT "COM"

#define SC3_DEFAULT_COMMENT "Game-only:no blends/cam/lights"

#define SC3_FILE_NAME_DEFAULT_SIZE SC_POLICY_FILE_NAME_MAX

#define SC3_MAIN_MESH_CHUNK_FIRST_POS 

#define SC3_MAIN_HAVE_SKIN 1

#define SC3_MAIN_TABLE_POS 11

#define SC3_TABLE_META_LEN 3

#define SC3_CHUNK_LEN 3

#define SC3_SCI_TEXTURE_OPTIONAL (1 << 0)
#define SC3_SCI_TEXTURE_DYNAMIC_OPTIONAL (1 << 1)
#define SC3_SCI_TEXTURE_STATIC_OPTIONAL (1 << 2)
#define SC3_BONE_MAP_OPTIONAL (1 << 1)


#define SC3_FILE_OUT_CODE SC_POLICY_OUT_NORMAL
#define SC3_FILE_W_FOPEN SC_POLICY_ERROR_FOPEN
#define SC3_FILE_R_FOPEN SC_POLICY_ERROR_FOPEN
#define SC3_FILE_READ_ENTRY_MAGIC_NUM SC_POLICY_ERROR_ENTRY_MAGIC_NUM
#define SC3_FILE_READ_USR SC_POLICY_ERROR_USER
#define SC3_FILE_READ_BEG SC_POLICY_ERROR_BEG
#define SC3_FILE_READ_END SC_POLICY_ERROR_END
#define SC3_FILE_READ_COM SC_POLICY_ERROR_COMMENT_MAIN
#define SC3_INVALID_VERSION SC_POLICY_ERROR_INVALID_VERSION
#define SC3_FILE_ERROR_CODE SC_POLICY_ERROR_IDENTIFY_START(1)
#define SC3_FILE_ERROR_VAR_NULL SC_POLICY_ERROR_IDENTIFY_START(2)
#define SC3_FILE_ERROR_MESH SC_POLICY_ERROR_IDENTIFY_START(3)
#define SC3_FILE_ERROR_ANIM SC_POLICY_ERROR_IDENTIFY_START(4)
#define SC3_FILE_ERROR_MAIN SC_POLICY_ERROR_IDENTIFY_START(5)
#define SC3_FILE_ERROR_TEX SC_POLICY_ERROR_IDENTIFY_START(6)
#define SC3_FILE_ERROR_TEX_INVALID_VERSION SC_POLICY_ERROR_IDENTIFY_START(7)
#define SC3_FREE_FUNC_API_NONE SC_POLICY_ERROR_IDENTIFY_START(8)
#define SC3_FILE_ERROR_MESH_TABLE SC_POLICY_ERROR_IDENTIFY_START(9)
#define SC3_FILE_ERROR_ANIM_TABLE SC_POLICY_ERROR_IDENTIFY_START(10)
#define SC3_FILE_ERROR_BONE_TABLE SC_POLICY_ERROR_IDENTIFY_START(11)
#define SC3_FILE_ERROR_TEX_TABLE SC_POLICY_ERROR_IDENTIFY_START(12)
#define SC3_FILE_ERROR_MAIN_TABLE SC_POLICY_ERROR_IDENTIFY_START(13)
#define SC3_FILE_ERROR_SUM_CHECK SC_POLICY_ERROR_IDENTIFY_START(14)
#define SC3_FILE_ERROR_MESH_DATA SC_POLICY_ERROR_IDENTIFY_START(15)
#define SC3_FILE_ERROR_MESH_INFORMATION SC_POLICY_ERROR_IDENTIFY_START(16)
#define SC3_FILE_ERROR_MESH_DATA__ SC_POLICY_ERROR_IDENTIFY_START(17)
#define SC3_FILE_ERROR_VIRTUAL_DATA SC_POLICY_ERROR_IDENTIFY_START(18)
#define SC3_FILE_ERROR_ANIM_INFORMATION SC_POLICY_ERROR_IDENTIFY_START(19)
#define SC3_FILE_ERROR_ANIM_DATA SC_POLICY_ERROR_IDENTIFY_START(20)
#define SC3_FILE_ERROR_BONE SC_POLICY_ERROR_IDENTIFY_START(21)
#define SC3_ANIM_INFORMATION_ERROR_FRAME SC_POLICY_ERROR_IDENTIFY_START(22)
#define SC3_ANIM_INFORMATION_ERROR_FRAMERATE SC_POLICY_ERROR_IDENTIFY_START(23)
#define SC3_FILE_ERROR_FILE_END_NEXT SC_POLICY_ERROR_IDENTIFY_START(24)
#define SC3_FILE_ERROR_MATCH_MESH_SKIN_DATA SC_POLICY_ERROR_IDENTIFY_START(25)
#define SC3_FILE_ERROR_MATCH_MESH_VERT_DATA SC_POLICY_ERROR_IDENTIFY_START(26)
#define SC3_FILE_ERROR_MATCH_ANIM_DEF_DATA SC_POLICY_ERROR_IDENTIFY_START(27)
#define SC3_FILE_ERROR_MATCH_ANIM_ROT_DATA SC_POLICY_ERROR_IDENTIFY_START(28)
#define SC3_FILE_ERROR_MATCH_ANIM_POS_DATA SC_POLICY_ERROR_IDENTIFY_START(29)
#define SC3_FILE_ERROR_MATCH_ANIM_SCL_DATA SC_POLICY_ERROR_IDENTIFY_START(30)
#define SC3_FILE_ERROR_MATCH_BONE_DATA SC_POLICY_ERROR_IDENTIFY_START(31)
#define SC3_FILE_ERROR_MATCH_SKELETON_DATA SC_POLICY_ERROR_IDENTIFY_START(32)

#define CALC_SC3_TABLE_POS(NUM__) SC3_MAIN_TABLE_POS + sizeof(sc_size) * (NUM__) + SC3_TABLE_META_LEN * (NUM__ + 1)
/**str  pos
 * 3     8 -> 11
*/
#define SC3_MESH_TABLE_POS CALC_SC3_TABLE_POS(0)
#define SC3_BONE_TABLE_POS CALC_SC3_TABLE_POS(1)
#define SC3_ANIM_TABLE_POS CALC_SC3_TABLE_POS(2)
#define SC3_TEX_TABLE_POS  CALC_SC3_TABLE_POS(3)


#define SC3_OFFSET_INF_NUM (SC_POLICY_SC_UINT_8_BITS  - 1)
#define SC3_HALF_UINT_8_BITS (SC_POLICY_SC_UINT_8_BITS / 2)

#define SC3_DEFORM_ROT_STATIC 1 << 0
#define SC3_DEFORM_POS_STATIC 1 << 1
#define SC3_DEFORM_SCALE_STATIC 1 << 2

#define SC3_CHECK_DYNAMIC_ROT(status) !(SC3_DEFORM_ROT_STATIC & status)
#define SC3_CHECK_DYNAMIC_POS(status) !(SC3_DEFORM_POS_STATIC & status)
#define SC3_CHECK_DYNAMIC_SCL(status) !(SC3_DEFORM_SCALE_STATIC & status)

#define SC3_COMMENT_SIZE_MAX 35
#define SC3_FREE_DATA_SIZE (1 << 8)
#define SC3_SKIN_MAX_SIZE 4

#define SC3_NOW_VERSION SC_POLICY_SET_NOW_VERSION(SC_POLICY_BASE_VERSION)

#define SC3_DEFAULT_COMMENT_LEN SC_POLICY_OS_ANOTHER_COMMENT_SIZE(SC3_COMMENT_SIZE_MAX)


//#define SC3_DEBUG_VERSION 1.2.SC3_DEBUG_VERSION
#define SC3_DEBUG_VERSION_STR #SC3_DEBUG_VERSION
#define SC3_TEXTURE_VALID_VERSION SC_POLICY_SET_SC_VERSIONS(2, 0)


typedef union Speed_Charm_3D_Tags{
    char sc3_entry_magic_num[len_of_str(SC3_FILE_ENTRY_MAGIC_NUMBER)];
    //char sc3_ver[STRING_LITERAL_SIZE(SC3_VER)];
    char sc3_beg[len_of_str(SC3_MAIN_BEG)];
    char sc3_end[len_of_str(SC3_MAIN_END)];
    char sc3_usr[len_of_str(SC3_MAIN_USER)];
    char sc3_com[SC3_COMMENT_SIZE_MAX];
    char sc3_tbl[SC3_TABLE_META_LEN];
    char sc3_chunks[SC3_CHUNK_LEN];    
}Speed_Charm_3D_Tags;

typedef struct Speed_Charm_3D_Skin_Attr{
    sc_ubyte    skin_weight[SC3_SKIN_MAX_SIZE];
    sc_ushort   skin_indices[SC3_SKIN_MAX_SIZE];
}Speed_Charm_3D_Skin_Attr;

typedef struct Speed_Charm_3D_Mesh_Attr{
    SGG_Vector3 pos;
    SGG_Vector3 norm;
    SGG_Vector2 uv;
}Speed_Charm_3D_Mesh_Attr;

typedef struct Speed_Charm_3D_Mesh_Bone_Map{
    SGG_Mat4 bone_map;
    sc_uint bone_ind;
}Speed_Charm_3D_Mesh_Bone_Map;

struct Speed_Charm_3D_Skeleton_Node{SGG_Mat4 to_world;SGG_Mat4 to_parent;};

typedef struct Speed_Charm_3D_Skeleton{
    union{
        struct Speed_Charm_3D_Skeleton_Node node;
    };
    //struct{SGG_Mat4 inv_default_mat;SGG_Mat4 default_mat;};

    sc_int parent;
}Speed_Charm_3D_Skeleton;
typedef struct Speed_Charm_3D_Mesh{
    Speed_Charm_3D_Skin_Attr * skin_attr;
    Speed_Charm_3D_Mesh_Attr * mesh_attr;
    sc_uint *                  indices;
    sc_size                    attr_num;
    sc_size                    ind_num;
    sc_ubyte                    mesh_inf;
    //Speed_Charm_3D_Mesh_Bone_Map * mesh_bone_map;
    //sc_size mesh_bone_map_num;
}Speed_Charm_3D_Mesh;

typedef struct Speed_Charm_3D_Anim_Deform{
    sc_ubyte deform_status; //
    SGG_Vector4 * rot;
    SGG_Vector3 * pos;
    SGG_Vector3 * scale;
}Speed_Charm_3D_Anim_Deform;
typedef struct Speed_Charm_3D_Anim{
    float time_beg;
    float time_end;
    float framerate;
    sc_size frame_num;
    //sc_ubyte * deform_status; // A
    Speed_Charm_3D_Anim_Deform * anim_deforms;
}Speed_Charm_3D_Anim;
typedef struct Speed_Charm_3D_Bone_Map{
    SGG_Mat4 inverse_mat;
    sc_uint ind;
}Speed_Charm_3D_Bone_Map;
typedef struct Speed_Charm_3D_Main{
    Speed_Charm_3D_Mesh * sc_meshes;
    Speed_Charm_3D_Anim * sc_anim;
    Speed_Charm_3D_Bone_Map * sc_bones;
    Speed_Charm_3D_Skeleton * sc_skeletons;
    sc_size               skeletons_num;
    sc_size               meshes_num;
    sc_size               bones_num;
    sc_size               anim_num;
}Speed_Charm_3D_Main;

typedef struct Speed_Charm_3D_Main_Contents{
    SC_FILE sc3_f__;
    sc_ushort sc3_version;
    uint8_t sc3_optional;

    struct{
        sc_size mesh_pos;
        sc_size bone_pos;
        sc_size anim_pos;
        sc_size tex_pos;
    };
    Speed_Charm_3D_Main * sc3_main;
}Speed_Charm_3D_Main_Contents;

/**
 * @brief Implementation-defined.
 * The structure layout and behavior are left to the implementation.
 */
struct Speed_Charm_3D_Virtual_Vertex_Package;

typedef struct Speed_Charm_3D_Run_Mesh{
    //Speed_Charm_3D_Mesh_Bone_Map * mesh_bone_map;
    //sc_size mesh_bone_map_num;
    sc_ubyte mesh_inf;
}Speed_Charm_3D_Run_Mesh;

typedef struct Speed_Charm_3D_Run_Game_Data{
    SGG_Vector3 pos;
    SGG_Vector3 scale;
}Speed_Charm_3D_Run_Game_Data;
typedef struct Speed_Charm_3D_Run_Main{
    sc_size meshes_num;
    sc_size bones_num;
    sc_size skeletons_num;
    sc_size anim_num;
    struct Speed_Charm_3D_Virtual_Vertex_Package * sc_vvp;
    Speed_Charm_3D_Run_Mesh * sc_meshes;
    Speed_Charm_3D_Anim * sc_anim;
    Speed_Charm_3D_Bone_Map * sc_bones;
    Speed_Charm_3D_Skeleton * sc_skeletons;
    float anim_times;
}Speed_Charm_3D_Run_Main;


typedef struct Speed_Charm_3D_Virtual_Vertex_Package * virtual_setter_vvp_func(Speed_Charm_3D_Main * sc3_main);

typedef virtual_setter_vvp_func * create_sc3_vvp_api_funcs;

typedef struct Speed_Charm_3D_Virtual_Vertex_Package * virtual_free_vvp_func(struct Speed_Charm_3D_Virtual_Vertex_Package * sc3_vvp);

typedef virtual_free_vvp_func * free_vvp_api_funcs;

_EXTERN_C_BEGIN_

#ifdef ANIM_DEFORM_STATUS_DEBUG
speed_charm_3d_si_strap sc_ubyte get_animation_deform_status(sc_size d_idx, Speed_Charm_3D_Anim * sc3_anim){
    
    const sc_size idx__ = d_idx / 2;
    const sc_ubyte dfs__ = sc3_anim->deform_status[idx__];
    return d_idx % 2 ? dfs__ >> 4 : ((dfs__ << 4) >> 4);
}

#else

speed_charm_3d_si_strap sc_ubyte get_animation_deform_status(sc_size d_idx, const Speed_Charm_3D_Anim * sc3_anim){
    return sc3_anim->anim_deforms[d_idx].deform_status;
}

speed_charm_3d_si_strap sc_ubyte get_animation_deform_status_opt(sc_size d_idx, Speed_Charm_3D_Anim * sc3_anim){
    const sc_size idx__ = d_idx / 2;    
    const sc_ubyte dfs__ = sc3_anim->anim_deforms[idx__].deform_status;
    return d_idx % 2 ? dfs__ >> 4 : ((dfs__ << 4) >> 4);

}

#endif

speed_charm_3d_si_strap bool smart_sc3_tags_reads(char * dst, char * src, sc_size s_size, SC_FILE sc3_f){
    scl_fread(sc3_f, dst, sc_byte, s_size);
    return equal_string(dst, src, s_size);
}
sc_err_t delete_speed_charm_3d_main(Speed_Charm_3D_Main ** sc3_main);
sc_err_t delete_speed_charm_3d_main_contents(Speed_Charm_3D_Main_Contents ** sc3_main_cont);
sc_err_t delete_speed_charm_3d_run_main(Speed_Charm_3D_Run_Main ** sc3_run_main, free_vvp_api_funcs s_free_vvp);

sc_err_t delete_invalid_data(Speed_Charm_3D_Main * sc3_main_cont);

Speed_Charm_3D_Run_Main * change_sc3_main_to_sc3_run_main(Speed_Charm_3D_Main ** sc3_main, struct Speed_Charm_3D_Virtual_Vertex_Package * vvp);

void set_sc3_file_format(sc_byte * sc3_path);

void goto_sc3f_pos(SC_FILE __sc3f, sc_size pos);
Speed_Charm_3D_Main_Contents * init_speed_charm_3d_main_contents(Speed_Charm_3D_Main * sc3_main);
sc_err_t write_speed_charm_3d_main(const sc_byte * sc3_file_path, Speed_Charm_3D_Main * sc3_main);
sc_err_t write_speed_charm_3d_data_main(Speed_Charm_3D_Main_Contents * sc3_main_cont);
sc_err_t write_speed_charm_3d_file_entry(Speed_Charm_3D_Main_Contents * sc3_main_cont, const sc_byte * sc3_file_path,sc_ushort version);

sc_err_t write_speed_charm_3d(const sc_byte * sc3_file_path, Speed_Charm_3D_Main * sc3_main);
sc_err_t write_speed_charm_3d_main_table(Speed_Charm_3D_Main_Contents * sc3_main_cont);
sc_err_t write_speed_charm_3d_user(Speed_Charm_3D_Main_Contents * sc3_main_cont, const sc_byte * comment_str);
sc_err_t write_speed_charm_3d_anim(Speed_Charm_3D_Main_Contents * sc3_main_cont, sc_size * end_size);
sc_err_t write_speed_charm_3d_bone(Speed_Charm_3D_Main_Contents * sc3_main_cont, sc_size * end_size);

sc_err_t write_speed_charm_3d_mesh(Speed_Charm_3D_Main_Contents * sc3_main_cont, sc_size * end_size);
sc_err_t write_speed_charm_3d_tex(Speed_Charm_3D_Main_Contents * sc3_main_cont,  sc_size * end_size);

sc_err_t read_speed_charm_3d_main(Speed_Charm_3D_Main ** sc3_main, const sc_byte * sc3_path);
sc_err_t read_speed_charm_3d_entry_data(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags,const sc_byte * sc3_path);
sc_err_t read_speed_charm_3d_table(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags);
sc_err_t read_speed_charm_3d_user(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags);
sc_err_t read_speed_charm_3d_close(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags);
sc_err_t read_speed_charm_3d_data(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags);
sc_err_t read_speed_charm_3d_mesh(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags);
sc_err_t read_speed_charm_3d_anim(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags);
sc_err_t read_speed_charm_3d_bone(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags);
sc_err_t read_speed_charm_3d_tex(Speed_Charm_3D_Main_Contents * sc3_main_cont, Speed_Charm_3D_Tags * sc3_tags);

/*implement to animation*/


speed_charm_3d_si_strap bool have_skin_check(Speed_Charm_3D_Run_Mesh * sc3_mesh){return sc3_mesh->mesh_inf & SC3_MAIN_HAVE_SKIN;}
speed_charm_3d_si_strap Speed_Charm_3D_Skeleton * get_skelton_node(Speed_Charm_3D_Run_Main * sc3_main, sc_size idx){return &sc3_main->sc_skeletons[sc3_main->sc_bones[idx].ind];}
void start_animation_speed_charm_3d_bone(Speed_Charm_3D_Run_Main * sc3_main, sc_uint anim_num, float delta_time);
void start_speed_charm_3d_bones_hierarchy(Speed_Charm_3D_Run_Main * sc3_main);
void start_build_bones_speed_charm_3d(Speed_Charm_3D_Run_Main * sc3_main, Speed_Charm_3D_Bone_Map * sc3_mesh_bone_map, SGG_Mat4 * sc3_mat4);

_EXTERN_C_END_
#endif //_SC3_H_
