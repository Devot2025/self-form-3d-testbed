#ifndef _FBX_TO_SPEED_CHARM_3D_H_
#define _FBX_TO_SPEED_CHARM_3D_H_

#include <ufbx.h>
#include "speed_charm_3d.h"
#define fbx_to_sc3_si_strap static inline
#define FBX_NORMAL_FRAMERATE 30
#define FBX_MAX_FRAMERATE 4096


typedef struct Speed_Charm_3D_Bone_Map_Cache{
    sc_size bone_map_num;
    Speed_Charm_3D_Bone_Map * bones;
}Speed_Charm_3D_Bone_Map_Cache;

_EXTERN_C_BEGIN_

fbx_to_sc3_si_strap void count_fbx_mesh_part(ufbx_mesh * fbx_mesh, sc_uint * dst_triangle_num, sc_uint * dst_parts_num){
    for(size_t mp_idx = 0; mp_idx < fbx_mesh->material_parts.count; mp_idx++){
        ufbx_mesh_part * fbx_parts = &fbx_mesh->material_parts.data[mp_idx];
        if(fbx_parts->num_triangles == 0)continue;
        *dst_triangle_num += fbx_parts->num_triangles;
        ++(*dst_parts_num);
    }
}

fbx_to_sc3_si_strap void change_fbx_mat_to_sc3_mesh_mat(SGG_Mat4 * dst_mat4, ufbx_matrix * src_mat){
    
    INSERT_COL_MAT4(dst_mat4, 0, src_mat->m00, src_mat->m01, src_mat->m02, src_mat->m03);
    INSERT_COL_MAT4(dst_mat4, 1, src_mat->m10, src_mat->m11, src_mat->m12, src_mat->m13);
    INSERT_COL_MAT4(dst_mat4, 2, src_mat->m20, src_mat->m21, src_mat->m22, src_mat->m23);
    INSERT_COL_MAT4(dst_mat4, 3,         0.0f,         0.0f,         0.0f,         1.0f);

    //dst_mat4->m[0] = src_mat->m00; dst_mat4->m[1] = src_mat->m10; dst_mat4->m[2] = src_mat->m20; dst_mat4->m[3] = 0.0f;
    //dst_mat4->m[4] = src_mat->m01; dst_mat4->m[5] = src_mat->m11; dst_mat4->m[6] = src_mat->m21; dst_mat4->m[7] = 0.0f;
    //dst_mat4->m[8] = src_mat->m02; dst_mat4->m[9] = src_mat->m12; dst_mat4->m[10] = src_mat->m22; dst_mat4->m[11] = 0.0f;
    //dst_mat4->m[12] = src_mat->m03; dst_mat4->m[13] = src_mat->m13; dst_mat4->m[14] = src_mat->m23; dst_mat4->m[15] = 1.0f;
}

fbx_to_sc3_si_strap void change_fbx_vec3_to_sc3_vec3(SGG_Vector3 * dst_vec3, ufbx_vec3 * src_vec3){
    dst_vec3->x = src_vec3->x;
    dst_vec3->y = src_vec3->y;
    dst_vec3->z = src_vec3->z;
}

fbx_to_sc3_si_strap void change_fbx_vec4_to_sc3_vec4(SGG_Vector4 * dst_vec4, ufbx_quat * src_vec4){
    dst_vec4->x = src_vec4->x;
    dst_vec4->y = src_vec4->y;
    dst_vec4->z = src_vec4->z;
    dst_vec4->w = src_vec4->w;
}
Speed_Charm_3D_Main * change_fbx_speed_charm_3d(const char * fbx_file_path, float default_size);
ufbx_scene * loading_simple_fbx(const char * fbx_file_path, float default_size);
void change_fbx_speed_charm_3d_anim(Speed_Charm_3D_Anim * sc3_anim, ufbx_node ** fbx_node, sc_size node_size, ufbx_anim_stack * fbx_anim);
void change_fbx_speed_charm_3d_bones_anim(Speed_Charm_3D_Anim_Deform * sc3_anim_def, Speed_Charm_3D_Anim * sc3_anim, ufbx_anim_stack * fbx_anim_stack, ufbx_node * fbx_node);
void change_fbx_speed_charm_3d_mesh(Speed_Charm_3D_Mesh * sc3_mesh, Speed_Charm_3D_Bone_Map_Cache * bone_list, ufbx_mesh * fbx_mesh);
void change_fbx_attribute_to_sc3_attribute(Speed_Charm_3D_Mesh * sc3_mesh, Speed_Charm_3D_Bone_Map_Cache * bone_list, ufbx_mesh * fbx_mesh, ufbx_skin_deformer * fbx_skin, sc_uint fbx_triangle_num, Speed_Charm_3D_Skin_Attr * skin_attribute);
_EXTERN_C_END_

#endif //_FBX_TO_SPEED_CHARM_3D_H_