cmake_minimum_required(VERSION 3.21)
project(sc_engine C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

set(OpenGL_GL_PREFERENCE "GLVND")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)

set(Game_Engine_Top_Path ${CMAKE_CURRENT_SOURCE_DIR})

# ===============================
# sc_engine static library
# ===============================
add_library(sc_engine STATIC
  "${Game_Engine_Top_Path}/graphic_engine/glsys_core.cpp"
  "${Game_Engine_Top_Path}/graphic_engine/shader_loader.cpp"
  "${Game_Engine_Top_Path}/graphic_engine/glsys_camera.cpp"
  "${Game_Engine_Top_Path}/graphic_engine/glsys_texture.cpp"
  "${Game_Engine_Top_Path}/graphic_engine/glsys_image_loader.cpp"
  "${Game_Engine_Top_Path}/graphic_engine/glsys_standard_obj_ele.cpp"
  "${Game_Engine_Top_Path}/ggmath/sgg_vec.c"
  "${Game_Engine_Top_Path}/external/glad/src/glad.c"
  "${Game_Engine_Top_Path}/sc3_loader/speed_charm_3d.c"
  "${Game_Engine_Top_Path}/src/prog_c/stb_link_helper.c"
  "${Game_Engine_Top_Path}/src/prog_c/std_ext/ext_strs.c"
)

target_include_directories(sc_engine
  PUBLIC
    "${Game_Engine_Top_Path}"
    "${Game_Engine_Top_Path}/graphic_engine"
    "${Game_Engine_Top_Path}/font_engine"
    "${Game_Engine_Top_Path}/external/ufbx"
    "${Game_Engine_Top_Path}/external/stb-master"
    "${Game_Engine_Top_Path}/src"
    "${Game_Engine_Top_Path}/src/prog_c"
    "${Game_Engine_Top_Path}/src/prog_cpp"
    "${Game_Engine_Top_Path}/external/glad/include"
)

target_link_libraries(sc_engine
  PUBLIC
    glfw
    OpenGL::GL
    OpenGL::GLU
)

# ===============================
# executable
# ===============================
add_executable(sc_engine_app
  main.cpp
  test_game_1_contents_main.cpp
  test_game_1_optional_main.cpp
)

target_link_libraries(sc_engine_app
  PRIVATE
    sc_engine
)
