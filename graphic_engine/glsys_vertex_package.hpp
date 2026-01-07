
#ifndef _GLSYS_VERTEX_PACKAGE_HPP_
#define _GLSYS_VERTEX_PACKAGE_HPP_
#include "glsys_core.hpp"
template <GLuint Vbo_Num, GLenum Object_Type>
class Basic_Gl_Vertex_Package{
    public:
    Basic_Gl_Vertex_Package();
    ~Basic_Gl_Vertex_Package();
    protected:
    GLuint vao;
    GLuint vbo[Vbo_Num];
    GLuint vtn;
    inline void gen_vertex_array_object(){
        glGenVertexArrays(1, &vao);
    }
    public:
    void gen_vp(GLuint gen_vbo_num = Vbo_Num);
    inline void start_vp(){
		glBindVertexArray(vao);
    }
    public:
    template <typename Send_Ty = float>
    void set_up_vp_buffer_on_gpu(const GLuint vbo_index, const Send_Ty * buffer = nullptr, GLuint buffer_num = 0);
    void set_up_vp_ebo_on_gpu(const GLuint ebo_index, const GLuint * indices_buffer, const GLuint indices_buffer_num);
    template <typename Send_Ty = float, typename Attribute_Data = Send_Ty, GLuint Normalize_Check = GL_FALSE>
    void set_up_vp_buffer_shader_attribute_memory(GLuint var_location_id, GLuint attribute_dimension, GLuint per_elements_dimension, GLuint offset_pos);
    template <typename Send_Ty = float, typename Attribute_Data = Send_Ty>
    void set_up_vp_buffer_shader_attribute_memory_int(GLuint var_location_id,GLuint attribute_dimension, GLuint per_elements_dimension, uintptr_t offset_pos);

    template <typename Min_Type = float, GLuint Normalize_Check = GL_FALSE, typename Data_Types, typename Offset_Ty>
    void set_up_vp_buffer_shader_attribute_memory_s(Offset_Ty Data_Types:: * d_offset, GLuint var_location_id, GLuint attribute_dimension, GLuint per_elements_dimension = 1);
    template <typename Min_Type = float, typename Data_Types, typename Offset_Ty>
    void set_up_vp_buffer_shader_attribute_memory_is(Offset_Ty Data_Types:: * d_offset, GLuint var_location_id, GLuint attribute_dimension, GLuint per_elements_dimension = 1);

    public:
    inline void start_normal_draw(GLuint a_vtn){
        start_vp();
        glDrawArrays(GL_TRIANGLES, 0, a_vtn);
    }
    inline void start_normal_draw(){
        start_vp();
        glDrawArrays(GL_TRIANGLES, 0, vtn);
    }
    inline void e_draw(){
        start_vp();
        glDrawElements(GL_TRIANGLES, vtn, GL_UNSIGNED_INT, nullptr);
    }
};


template <GLuint Vbo_Num, GLenum Object_Type>
template <typename Min_Type, typename Data_Types, typename Offset_Ty>
void Basic_Gl_Vertex_Package<Vbo_Num, Object_Type>::set_up_vp_buffer_shader_attribute_memory_is(Offset_Ty Data_Types:: * d_offset, GLuint var_location_id, GLuint attribute_dimension, GLuint per_elements_dimension){
    
    //size_t offset_pos = reinterpret_cast<size_t>(&(reinterpret_cast<Data_Types const volatile*>(0)->*d_offset));
    
    Data_Types tmp_data_types{};
    auto tmp_data_ptr = &tmp_data_types;
    auto offset_ptr = &(tmp_data_ptr->*(d_offset));
    size_t result__ = reinterpret_cast<size_t>(offset_ptr) - reinterpret_cast<size_t>(tmp_data_ptr);
    /**
     * Data Types The Array standard
     * 
     * Array
     * {
     * a, a, a, b, b, b, c, c
     * ....
     * }
    */
   
    glVertexAttribIPointer(var_location_id, attribute_dimension, fix_gl_type<Min_Type>(), sizeof(Data_Types) * per_elements_dimension, (void *)(result__));
    glEnableVertexAttribArray(var_location_id);

}
template <GLuint Vbo_Num, GLenum Object_Type>
template <typename Min_Type, GLuint Normalize_Check, typename Data_Types, typename Offset_Ty>
void Basic_Gl_Vertex_Package<Vbo_Num, Object_Type>::set_up_vp_buffer_shader_attribute_memory_s(Offset_Ty Data_Types:: * d_offset, GLuint var_location_id, GLuint attribute_dimension, GLuint per_elements_dimension){
    
    //size_t offset_pos = reinterpret_cast<size_t>(&(reinterpret_cast<Data_Types const volatile*>(0)->*d_offset));
    Data_Types tmp_data_types{};
    auto tmp_data_ptr = &tmp_data_types;
    auto offset_ptr = &(tmp_data_ptr->*(d_offset));
    size_t result__ = reinterpret_cast<size_t>(offset_ptr) - reinterpret_cast<size_t>(tmp_data_ptr);
    /**
     * Data Types The Array standard
     * 
     * Array
     * {
     * a, a, a, b, b, b, c, c
     * ....
     * }
    */
    glVertexAttribPointer(var_location_id, attribute_dimension, fix_gl_type<Min_Type>(), Normalize_Check, sizeof(Data_Types) * per_elements_dimension, (void *)(result__));
    glEnableVertexAttribArray(var_location_id);

}
template <GLuint Vbo_Num, GLenum Object_Type>
template <typename Send_Ty, typename Attribute_Data>
void Basic_Gl_Vertex_Package<Vbo_Num, Object_Type>::set_up_vp_buffer_shader_attribute_memory_int(GLuint var_location_id,GLuint attribute_dimension, GLuint per_elements_dimension, uintptr_t offset_pos){
    glVertexAttribIPointer(var_location_id, attribute_dimension, fix_gl_type<Send_Ty>(), sizeof(Attribute_Data) * per_elements_dimension, (void *)(offset_pos));
    glEnableVertexAttribArray(var_location_id);
}
template <GLuint Vbo_Num, GLenum Object_Type>
template <typename Send_Ty, typename Attribute_Data, GLuint Normalize_Check>
void Basic_Gl_Vertex_Package<Vbo_Num, Object_Type>::set_up_vp_buffer_shader_attribute_memory(GLuint var_location_id, GLuint attribute_dimension, GLuint per_elements_dimension, GLuint offset_pos){
    glVertexAttribPointer(var_location_id, attribute_dimension, fix_gl_type<Send_Ty>(), Normalize_Check, sizeof(Attribute_Data) * per_elements_dimension, (void *)(sizeof(Send_Ty) * offset_pos));
    glEnableVertexAttribArray(var_location_id);
}
template <GLuint Vbo_Num, GLenum Object_Type>
Basic_Gl_Vertex_Package<Vbo_Num, Object_Type>::Basic_Gl_Vertex_Package() : vao{}, vtn{}{
    for(GLuint i = 0; i < Vbo_Num; ++i)vbo[i] = 0;
}

template <GLuint Vbo_Num, GLenum Object_Type>
Basic_Gl_Vertex_Package<Vbo_Num, Object_Type>::~Basic_Gl_Vertex_Package(){
    glDeleteBuffers(Vbo_Num, vbo);
    glDeleteVertexArrays(1, &vao);
}
template <GLuint Vbo_Num, GLenum Object_Type>
template <typename Send_Ty>
void Basic_Gl_Vertex_Package<Vbo_Num, Object_Type>::set_up_vp_buffer_on_gpu(const GLuint vbo_index, const Send_Ty * v_buffer, GLuint buffer_num){
    glBindBuffer(GL_ARRAY_BUFFER, vbo[vbo_index]);
    glBufferData(GL_ARRAY_BUFFER, buffer_num * sizeof(Send_Ty), v_buffer, Object_Type);
    vtn = buffer_num / 2;
}

template <GLuint Vbo_Num, GLenum Object_Type>
void Basic_Gl_Vertex_Package<Vbo_Num, Object_Type>::set_up_vp_ebo_on_gpu(const GLuint ebo_index,const GLuint * indices_buffer, const GLuint indices_buffer_num){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[ebo_index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_buffer_num * sizeof(GLuint), indices_buffer, Object_Type);
    vtn = (size_t)indices_buffer_num;
}
template <GLuint Vbo_Num, GLenum Object_Type>
void Basic_Gl_Vertex_Package<Vbo_Num, Object_Type>::gen_vp(GLuint gen_vbo_num){
    gen_vertex_array_object();
    start_vp();

    glGenBuffers(gen_vbo_num, vbo);
}

#endif // _GLSYS_VERTEX_PACKAGE_HPP_

