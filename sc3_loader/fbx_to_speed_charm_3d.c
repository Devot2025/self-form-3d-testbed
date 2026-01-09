#include "fbx_to_speed_charm_3d.h"




ufbx_scene * loading_simple_fbx(const char * fbx_file_path, float default_size){
    ufbx_load_opts __opts = {0};
    __opts.target_unit_meters = default_size;
    ufbx_error load_err;
    ufbx_scene * __scene = ufbx_load_file(fbx_file_path, &__opts, &load_err);
    //printf("%p %s\n", __scene, fbx_file_path);
    return __scene;
}

Speed_Charm_3D_Main * change_fbx_speed_charm_3d(const char * fbx_file_path, float default_size){
    if(default_size != 0.0f) default_size = 1.0f / default_size;
    else default_size = 1.0f;
    if(!fbx_file_path) return NULL;
    ufbx_scene * f_scene = loading_simple_fbx(fbx_file_path, default_size);
    if(!f_scene) return NULL;
    Speed_Charm_3D_Main * sc3_main = smart_calloc(Speed_Charm_3D_Main, 1);
    if(!sc3_main) return NULL;
    sc3_main->meshes_num = f_scene->meshes.count;
    sc3_main->sc_meshes = smart_calloc(Speed_Charm_3D_Mesh, f_scene->meshes.count);
    Speed_Charm_3D_Bone_Map_Cache * bone_list = smart_calloc(Speed_Charm_3D_Bone_Map_Cache , sc3_main->meshes_num);
    for(sc_size i = 0; i < sc3_main->meshes_num; ++i) change_fbx_speed_charm_3d_mesh(&sc3_main->sc_meshes[i], &bone_list[i], f_scene->meshes.data[i]);
    
    sc3_main->skeletons_num = f_scene->nodes.count;
    sc3_main->sc_skeletons = smart_calloc(Speed_Charm_3D_Skeleton, sc3_main->skeletons_num);
    for(sc_size i = 0; i < sc3_main->skeletons_num; ++i){
        sc3_main->sc_skeletons[i].parent = f_scene->nodes.data[i]->parent ? f_scene->nodes.data[i]->parent->typed_id : -1;
        change_fbx_mat_to_sc3_mesh_mat(&sc3_main->sc_skeletons[i].node.to_world , &f_scene->nodes.data[i]->node_to_world);
        change_fbx_mat_to_sc3_mesh_mat(&sc3_main->sc_skeletons[i].node.to_parent , &f_scene->nodes.data[i]->node_to_parent);
        //change_fbx_mat_to_sc3_mesh_mat(&sc3_main->sc_bones[i].skl_inv , &f_scene->nodes.data[i]->node_to_parent);
    }

    sc3_main->anim_num = f_scene->anim_stacks.count;
    
    sc3_main->sc_anim = smart_calloc(Speed_Charm_3D_Anim, sc3_main->anim_num);
    
    for(sc_size i = 0; i < sc3_main->anim_num; ++i) change_fbx_speed_charm_3d_anim(&sc3_main->sc_anim[i], f_scene->nodes.data, f_scene->nodes.count, f_scene->anim_stacks.data[i]);
    
    Speed_Charm_3D_Bone_Map * sc3_bone_map__;
    
    sc_size total_bone_map__ = 0;
    
    for(sc_size i = 0; i < sc3_main->meshes_num; ++i) total_bone_map__ += bone_list[i].bone_map_num;
    
    sc3_bone_map__ = smart_calloc(Speed_Charm_3D_Bone_Map, total_bone_map__);
    
    sc_size idx_bone_map__ = 0;
    
    if(!sc3_bone_map__) return sc3_main;
    //tmp_printf_e("%d\n", total_bone_map__);
    for(sc_size i = 0; i < sc3_main->meshes_num; ++i) {
        for(sc_size j = 0; j < bone_list[i].bone_map_num; ++j){
            Speed_Charm_3D_Bone_Map sc3_mesh_bone__ = bone_list[i].bones[j];
            if(!idx_bone_map__) sc3_bone_map__[idx_bone_map__++] = sc3_mesh_bone__;
            else {
                bool check__ = false;
                for(sc_size k = 0; k < idx_bone_map__; ++k){
                    Speed_Charm_3D_Bone_Map sc3_mesh_bone = sc3_bone_map__[k];
                    if(memcmp(&sc3_mesh_bone__.inverse_mat, &sc3_mesh_bone.inverse_mat, sizeof(SGG_Mat4)) == 0){
                        if(sc3_mesh_bone__.ind == sc3_mesh_bone.ind){
                            check__ = true;
                            break;
                        }
                    }
                }
                if(!check__) sc3_bone_map__[idx_bone_map__++] = sc3_mesh_bone__;
            }        
        
        
        
            //printf("[%d]", sc3_mesh_bone__.bone_ind);
        } 
    }
    for(sc_size i = 0; i < sc3_main->meshes_num; ++i){
    Speed_Charm_3D_Mesh *ms = &sc3_main->sc_meshes[i];
    Speed_Charm_3D_Bone_Map_Cache *sk = &bone_list[i];

    if(!(ms->mesh_inf & SC3_MAIN_HAVE_SKIN) || !ms->skin_attr) continue;

    for(sc_size v = 0; v < ms->attr_num; ++v){
        Speed_Charm_3D_Skin_Attr *sv = &ms->skin_attr[v];

        for(sc_size t = 0; t < 4; ++t){
            if(!sv->skin_weight[t]) continue;

            sc_ushort local = sv->skin_indices[t];
            if(local >= sk->bone_map_num){
                sv->skin_weight[t] = 0;
                sv->skin_indices[t] = 0;
                continue;
            }
            Speed_Charm_3D_Bone_Map bone_map__ = sk->bones[local];

            for(sc_size h = 0; h < idx_bone_map__; ++h){
                Speed_Charm_3D_Bone_Map global__ = sc3_bone_map__[h];

                    if(memcmp(&global__.inverse_mat, &bone_map__.inverse_mat, sizeof(SGG_Mat4)) == 0){
                        if(global__.ind == bone_map__.ind){
                            sv->skin_indices[t] = (sc_ushort)h;
                            break;
                        }
                    }
                }
            }
        }
    }
    //s_free(node_list);

    sc3_main->bones_num = idx_bone_map__;
    sc3_main->sc_bones = sc3_bone_map__;
    for(sc_size i = 0; i < sc3_main->meshes_num; ++i){
        s_free(bone_list[i].bones);
    }
    s_free(bone_list);
    ufbx_free_scene(f_scene);

    return sc3_main;
}

void change_fbx_speed_charm_3d_bones_anim(Speed_Charm_3D_Anim_Deform * sc3_anim_def, Speed_Charm_3D_Anim * sc3_anim, ufbx_anim_stack * fbx_anim_stack, ufbx_node * fbx_node){
    sc3_anim_def->rot = smart_calloc(SGG_Vector4, sc3_anim->frame_num);
    sc3_anim_def->pos = smart_calloc(SGG_Vector3, sc3_anim->frame_num);
    sc3_anim_def->scale = smart_calloc(SGG_Vector3, sc3_anim->frame_num);
    sc3_anim_def->deform_status = SC3_DEFORM_ROT_STATIC | SC3_DEFORM_POS_STATIC | SC3_DEFORM_SCALE_STATIC; 
    //sc3_anim_def->cpos = sc3_anim_def->crot = sc3_anim_def->cscale = true;
    //bool rot_check = true;
	for (size_t i = 0; i < sc3_anim->frame_num; i++) {
		double time = fbx_anim_stack->time_begin + (double)i / sc3_anim->framerate;
		ufbx_transform transform = ufbx_evaluate_transform(fbx_anim_stack->anim, fbx_node, time);
        change_fbx_vec4_to_sc3_vec4(&sc3_anim_def->rot[i], &transform.rotation);
        change_fbx_vec3_to_sc3_vec3(&sc3_anim_def->pos[i], &transform.translation);
        change_fbx_vec3_to_sc3_vec3(&sc3_anim_def->scale[i], &transform.scale);
        
		if (i > 0) {
			float dot = sgg_dot_vector4(&sc3_anim_def->rot[i], &sc3_anim_def->rot[i - 1]);
            if (dot < 0.0f) sgg_neg_vector4(&sc3_anim_def->rot[i]);

            bool qual_check = sgg_equal_vector4(&sc3_anim_def->rot[i], &sc3_anim_def->rot[i - 1]);
			if (!qual_check) sc3_anim_def->deform_status &= ~(SC3_DEFORM_ROT_STATIC);
            qual_check =  sgg_equal_vector3(&sc3_anim_def->pos[i], &sc3_anim_def->pos[i - 1]);
			if (!qual_check) sc3_anim_def->deform_status &= ~(SC3_DEFORM_POS_STATIC);
            qual_check =  sgg_equal_vector3(&sc3_anim_def->scale[i], &sc3_anim_def->scale[i - 1]);
			if (!qual_check) sc3_anim_def->deform_status &= ~(SC3_DEFORM_SCALE_STATIC);
		}
	}
    
	if (sc3_anim_def->deform_status & SC3_DEFORM_ROT_STATIC) { 
        SGG_Vector4 * static_rot = smart_realloc(SGG_Vector4, sc3_anim_def->rot, 1);
        if(static_rot) sc3_anim_def->rot = static_rot;
    }
	if (sc3_anim_def->deform_status & SC3_DEFORM_POS_STATIC) { 
        SGG_Vector3 * static_pos = smart_realloc(SGG_Vector3, sc3_anim_def->pos, 1);
        if(static_pos) sc3_anim_def->pos = static_pos;
    }
	if (sc3_anim_def->deform_status & SC3_DEFORM_SCALE_STATIC) { 
        SGG_Vector3 * static_scale = smart_realloc(SGG_Vector3, sc3_anim_def->scale, 1);
        if(static_scale) sc3_anim_def->scale = static_scale;
    }
}

void change_fbx_speed_charm_3d_anim(Speed_Charm_3D_Anim * sc3_anim, ufbx_node ** fbx_node, sc_size node_size, ufbx_anim_stack * fbx_anim){
    const float target_framerate = FBX_NORMAL_FRAMERATE;
    const size_t max_frames = FBX_MAX_FRAMERATE;
	float duration = (float)fbx_anim->time_end - (float)fbx_anim->time_begin;
    size_t frames_num = sgg_clampu((size_t)(duration * target_framerate), 2, max_frames);
    float framerate = (float)(frames_num - 1) / duration;
    sc3_anim->time_beg = (float)fbx_anim->time_begin;
    sc3_anim->time_end = (float)fbx_anim->time_end;
    sc3_anim->framerate = framerate;
    sc3_anim->frame_num = frames_num;
    sc3_anim->anim_deforms = smart_calloc(Speed_Charm_3D_Anim_Deform, node_size);
    for(sc_size i = 0; i < node_size; ++i){
        change_fbx_speed_charm_3d_bones_anim(&sc3_anim->anim_deforms[i], sc3_anim, fbx_anim, fbx_node[i]);
    }
}

void change_fbx_speed_charm_3d_mesh(Speed_Charm_3D_Mesh * sc3_mesh, Speed_Charm_3D_Bone_Map_Cache * bone_list, ufbx_mesh * fbx_mesh){
    sc_uint f_tri_num = 0;
    sc_uint f_parts_num = 0;
    count_fbx_mesh_part(fbx_mesh, &f_tri_num, &f_parts_num);
    Speed_Charm_3D_Skin_Attr * mesh_skin_attribute = smart_calloc(Speed_Charm_3D_Skin_Attr, fbx_mesh->num_vertices);
    ufbx_skin_deformer *skin_deformer = NULL;
    if(fbx_mesh->skin_deformers.count > 0){
        sc3_mesh->mesh_inf |= SC3_MAIN_HAVE_SKIN;

        assert(fbx_mesh->skin_deformers.count <= 1);
        skin_deformer = fbx_mesh->skin_deformers.data[0];
        for(sc_size v_idx = 0; v_idx < fbx_mesh->num_vertices; ++v_idx){
            sc_size weight_size = 0;
            float total_weight = 0.0f;
            float weights[4] = {0.0f};
            sc_ushort clusters[4] = {0};
            ufbx_skin_vertex mesh_skin_vertex = skin_deformer->vertices.data[v_idx];
            for(size_t w_idx = 0; w_idx < mesh_skin_vertex.num_weights; ++w_idx){
                if(weight_size >= 4){
                    break;
                }
                ufbx_skin_weight skin_weight = skin_deformer->weights.data[mesh_skin_vertex.weight_begin + w_idx];
                total_weight += (float)skin_weight.weight;
                clusters[weight_size] = (sc_ushort)skin_weight.cluster_index;
                weights[weight_size] = (float)skin_weight.weight;
                weight_size++;
            }
            if(total_weight > 0.0f){
                Speed_Charm_3D_Skin_Attr * now_skin_attribute = &mesh_skin_attribute[v_idx];
                sc_uint quantized_sum = 0;
                for(sc_uint i = 0; i < 4; ++i){
                    sc_ubyte quantized_weight = (sc_ubyte)((float)weights[i] / total_weight * 255.0f);
                    quantized_sum += quantized_weight;
                    now_skin_attribute->skin_indices[i] = clusters[i];
                    now_skin_attribute->skin_weight[i] = quantized_weight;
                }
                if (quantized_sum > 255) quantized_sum = 255;
                now_skin_attribute->skin_weight[0] += (sc_ubyte)(255 - quantized_sum);

            }
        }
    }
    change_fbx_attribute_to_sc3_attribute(sc3_mesh, bone_list, fbx_mesh, skin_deformer, f_tri_num, mesh_skin_attribute);
    /*
    if(skin_deformer){
        sc_size node_size = skin_deformer->clusters.count;
        for(sc_size i = 0; i < node_size; ++i){
            ufbx_node * nodes = skin_deformer->clusters.data[i]->bone_node;
            bool d_check = false;
            for(sc_size j = 0; j < *bone_num; ++j){
                if(nodes == (*node_list)[j]){
                    d_check = true;
                    break;
                }
            }
            if(d_check) continue;
            if(*bone_num >= *bone_size){
                ufbx_node ** __node = smart_realloc(ufbx_node * , *node_list, (*bone_num+ 10));
                if(!__node) {
                    s_free(mesh_skin_attribute);
                    return;    
                }
                *node_list = __node;
                *bone_size = *bone_num + 10;
            }
            (*node_list)[*bone_num] = nodes;
            (*bone_num)++;
        }
    }
    */
    s_free(mesh_skin_attribute);
}

void change_fbx_attribute_to_sc3_attribute(Speed_Charm_3D_Mesh * sc3_mesh, Speed_Charm_3D_Bone_Map_Cache * bone_map, ufbx_mesh * fbx_mesh, ufbx_skin_deformer * fbx_skin, sc_uint fbx_triangle_num, Speed_Charm_3D_Skin_Attr * skin_attribute){
    sc_uint * mesh_indices = smart_calloc(sc_uint, fbx_mesh->max_face_triangles * DIMENSION_XYZ);
    sc_uint * mesh_triangle_indices = smart_calloc(sc_uint, fbx_triangle_num * DIMENSION_XYZ);
    if(!mesh_indices)return;
    if(!mesh_triangle_indices)return;
    Speed_Charm_3D_Skin_Attr * sc3_skin_attr = smart_calloc(Speed_Charm_3D_Skin_Attr, fbx_triangle_num * fbx_mesh->material_parts.count * DIMENSION_XYZ);
    Speed_Charm_3D_Mesh_Attr * sc3_mesh_attr = smart_calloc(Speed_Charm_3D_Mesh_Attr, fbx_triangle_num * fbx_mesh->material_parts.count * DIMENSION_XYZ);
    
    for(sc_size mp_idx = 0; mp_idx < fbx_mesh->material_parts.count; ++mp_idx){
        ufbx_mesh_part * mesh_part = &fbx_mesh->material_parts.data[mp_idx];
        if(mesh_part->num_triangles == 0)continue;
        sc_uint atr_idx = 0;
        for(sc_size f_idx = 0; f_idx < mesh_part->num_faces; ++f_idx){
            
            ufbx_face mesh_face = fbx_mesh->faces.data[mesh_part->face_indices.data[f_idx]];
            sc_uint triangle_face_num = ufbx_triangulate_face(mesh_indices, fbx_mesh->max_face_triangles * DIMENSION_XYZ, fbx_mesh, mesh_face) * DIMENSION_XYZ;
            
            ufbx_vec2 deform_tri_uv = {0.0f};
            
            for(sc_uint ta_idx = 0; ta_idx < triangle_face_num; ++ta_idx){

                sc_uint tmp_indices_num = mesh_indices[ta_idx];
                ufbx_vec3 tri_vert = ufbx_get_vertex_vec3(&fbx_mesh->vertex_position, tmp_indices_num);
                ufbx_vec3 tri_norm = ufbx_get_vertex_vec3(&fbx_mesh->vertex_normal, tmp_indices_num);
                ufbx_vec2 tri_uv = fbx_mesh->vertex_uv.exists ? ufbx_get_vertex_vec2(&fbx_mesh->vertex_uv, tmp_indices_num) : deform_tri_uv;
                sc_size atr_idx_t = sc3_mesh->attr_num + atr_idx;
                sc3_mesh_attr[atr_idx_t].pos.x = tri_vert.x;
                sc3_mesh_attr[atr_idx_t].pos.y = tri_vert.y;
                sc3_mesh_attr[atr_idx_t].pos.z = tri_vert.z;
                sc3_mesh_attr[atr_idx_t].norm.x = tri_norm.x;
                sc3_mesh_attr[atr_idx_t].norm.y = tri_norm.y;   
                sc3_mesh_attr[atr_idx_t].norm.z = tri_norm.z;
                sc3_mesh_attr[atr_idx_t].uv.x = tri_uv.x;
                sc3_mesh_attr[atr_idx_t].uv.y = tri_uv.y;
                if(fbx_skin) sc3_skin_attr[atr_idx_t] = skin_attribute[fbx_mesh->vertex_indices.data[tmp_indices_num]];
                atr_idx++;
            }   
        }
        sc3_mesh->attr_num += atr_idx;
    }

    ufbx_vertex_stream v_stream[2];        
    size_t v_stream_num = 1;
    v_stream[0].data = sc3_mesh_attr;
    v_stream[0].vertex_count = sc3_mesh->attr_num;
    v_stream[0].vertex_size = sizeof(float) * 8;
    if(fbx_skin){
        v_stream_num = 2;
        v_stream[1].data = sc3_skin_attr;
        v_stream[1].vertex_count = sc3_mesh->attr_num;
        v_stream[1].vertex_size = sizeof(Speed_Charm_3D_Skin_Attr);
    }
    ufbx_error fbx_error;
    sc3_mesh->ind_num = sc3_mesh->attr_num;
    sc3_mesh->attr_num = ufbx_generate_indices(v_stream, v_stream_num, mesh_triangle_indices, sc3_mesh->ind_num, NULL, &fbx_error);
    if(fbx_error.type != UFBX_ERROR_NONE) return;
    sc3_mesh->mesh_attr = smart_calloc(Speed_Charm_3D_Mesh_Attr, sc3_mesh->attr_num);
    sc3_mesh->indices   = smart_calloc(sc_uint, sc3_mesh->ind_num);
    if(fbx_skin) sc3_mesh->skin_attr = smart_calloc(Speed_Charm_3D_Skin_Attr, sc3_mesh->attr_num);
    for(sc_size i = 0; i < sc3_mesh->attr_num; ++i){
        sc3_mesh->mesh_attr[i]              = sc3_mesh_attr[i];
        if(fbx_skin) sc3_mesh->skin_attr[i] = sc3_skin_attr[i];
    }
    for(sc_size i = 0; i < sc3_mesh->ind_num; ++i){
        sc3_mesh->indices[i] = mesh_triangle_indices[i];
    }
    
    if(fbx_skin){ 

        const sc_uint num_clusters = (sc_uint)fbx_skin->clusters.count;

        bool *weight_have_map = smart_calloc(bool, num_clusters);
        if(!weight_have_map)return;
        for (sc_uint i = 0; i < sc3_mesh->attr_num; ++i) {
            const Speed_Charm_3D_Skin_Attr *sv = &sc3_mesh->skin_attr[i];
            for (int j = 0; j < 4; ++j) {
                if (!sv->skin_weight[j]) continue;
                sc_ushort g = sv->skin_indices[j];                 
                if (g < num_clusters) weight_have_map[g] = true;
            }
        }
        sc_ushort *g2l = smart_calloc(sc_ushort, num_clusters);
        if(!g2l){
            free(weight_have_map);
        }
        /*init max uint16_t*/
        for (sc_uint g = 0; g < num_clusters; ++g) g2l[g] = 0xFFFF;
        bone_map->bones = smart_calloc(Speed_Charm_3D_Bone_Map ,num_clusters);
        if(!bone_map->bones){
            free(weight_have_map);
            free(g2l);
            return;
        }
        /*ps is fix map size and index*/
        sc_ushort ps = 0;
        /*marking is_valid weight cluster*/
        for (sc_uint g = 0; g < num_clusters; ++g) {
            if (!weight_have_map[g]) continue;
            g2l[g] = ps;
            ufbx_skin_cluster *cl = fbx_skin->clusters.data[g];
            bone_map->bones[ps].ind = (sc_uint)cl->bone_node->typed_id;
            
            change_fbx_mat_to_sc3_mesh_mat(&bone_map->bones[ps].inverse_mat, &cl->geometry_to_bone);
            ++ps;
        }

        bone_map->bone_map_num = ps;

        Speed_Charm_3D_Bone_Map * tmp_map = smart_realloc(Speed_Charm_3D_Bone_Map, bone_map->bones, ps);
        if(tmp_map) bone_map->bones = tmp_map;
        for (sc_err_t i = 0; i < sc3_mesh->attr_num; ++i) {
            Speed_Charm_3D_Skin_Attr * sv = &sc3_mesh->skin_attr[i];

            float    wf[4] = {0.0f};
            sc_err_t li[4] = {0};
            int      n = 0;
            float    sum = 0.0f;

            for (int j = 0; j < 4; ++j) {
                if (!sv->skin_weight[j]) continue;
                sc_short g = sv->skin_indices[j];                 
                if (g >= num_clusters) continue;
                sc_short l = g2l[g];                       
                if (l == 0xFFFF) continue;                 
                li[n] = l;
                wf[n] = sv->skin_weight[j] / 255.0f;
                sum  += wf[n];
                ++n; 
            }
            if (sum > 0.0f) {
                for (int t = 0; t < n; ++t) wf[t] /= sum;
            }
            sc_uint qsum = 0;
            for (int t = 0; t < 4; ++t) {
                sc_ubyte q = (t < n) ? (sc_ubyte)(wf[t] * 255.0f) : 0;
                sv->skin_indices[t]  = (t < n) ? li[t] : 0;
                sv->skin_weight[t] = q;
                qsum += q;
            }
            if(n > 0)
            sv->skin_weight[0] += (sc_ubyte)(255 - (qsum & 0xFF));
        }
    }
    
    s_free(sc3_mesh_attr);
    s_free(sc3_skin_attr);
    s_free(mesh_triangle_indices);
    s_free(mesh_indices);
}

#if TEST


if(fbx_skin){ 

    const sc_uint num_clusters = (sc_uint)fbx_skin->clusters.count;

    bool *weight_have_map = smart_calloc(bool, num_clusters);
    if(!weight_have_map)return;
    for (sc_uint i = 0; i < sc3_mesh->attr_num; ++i) {
        const Speed_Charm_3D_Skin_Attr *sv = &sc3_mesh->skin_attr[i];
        for (int j = 0; j < 4; ++j) {
            if (!sv->skin_weight[j]) continue;
            sc_ushort g = sv->skin_indices[j];                 
            if (g < num_clusters) weight_have_map[g] = true;
        }
    }
    sc_ushort *g2l = smart_calloc(sc_ushort, num_clusters);
    if(!g2l){
        free(weight_have_map);
    }
    /*init max uint16_t*/
    for (sc_uint g = 0; g < num_clusters; ++g) g2l[g] = 0xFFFF;
    sc3_mesh->tttt = smart_calloc(Speed_Charm_3D_Bone_Map_TEST ,num_clusters);
    if(!sc3_mesh->tttt){
        free(weight_have_map);
        free(g2l);
        return;
    }
    /*ps is fix map size and index*/
    sc_ushort ps = 0;
    /*marking is_valid weight cluster*/
    for (sc_uint g = 0; g < num_clusters; ++g) {
        if (!weight_have_map[g]) continue;
        g2l[g] = ps;
        ufbx_skin_cluster *cl = fbx_skin->clusters.data[g];
        sc3_mesh->tttt[ps].bone_ind = (sc_uint)cl->bone_node->typed_id;
        change_fbx_mat_to_sc3_mesh_mat(&sc3_mesh->tttt[ps].bone_map, &cl->geometry_to_bone);
        ++ps;
    }
    
    sc3_mesh->ttttSIZE = ps;

    Speed_Charm_3D_Bone_Map_TEST * tmp_map_indices = smart_realloc(Speed_Charm_3D_Bone_Map_TEST, sc3_mesh->tttt, ps);
    if(tmp_map_indices) sc3_mesh->tttt = tmp_map_indices;
    for (sc_err_t i = 0; i < sc3_mesh->attr_num; ++i) {
        Speed_Charm_3D_Skin_Attr * sv = &sc3_mesh->skin_attr[i];

        float    wf[4] = {0.0f};
        sc_err_t li[4] = {0};
        int      n = 0;
        float    sum = 0.0f;

        for (int j = 0; j < 4; ++j) {
            if (!sv->skin_weight[j]) continue;
            sc_short g = sv->skin_indices[j];                 
            if (g >= num_clusters) continue;
            sc_short l = g2l[g];                       
            if (l == 0xFFFF) continue;                 
            li[n] = l;
            wf[n] = sv->skin_weight[j] / 255.0f;
            sum  += wf[n];
            ++n; 
        }
        if (sum > 0.0f) {
            for (int t = 0; t < n; ++t) wf[t] /= sum;
        }
        sc_uint qsum = 0;
        for (int t = 0; t < 4; ++t) {
            sc_ubyte q = (t < n) ? (sc_ubyte)(wf[t] * 255.0f) : 0;
            sv->skin_indices[t]  = (t < n) ? li[t] : 0;
            sv->skin_weight[t] = q;
            qsum += q;
        }
        if(n > 0)
        sv->skin_weight[0] += (sc_ubyte)(255 - (qsum & 0xFF));
    }
    }
    
    #endif