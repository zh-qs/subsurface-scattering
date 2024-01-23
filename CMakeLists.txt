cmake_minimum_required(VERSION 3.12)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(cusymint LANGUAGES CXX C)

set(SRC_DIR "SubsurfaceScattering")
set(IMGUI_DIR "imgui")
set(GLAD_DIR "glad")

add_library(imgui
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)

add_library(glad
    ${GLAD_DIR}/glad.c
)

add_executable(SubsurfaceScattering
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/algebra.cpp
    ${SRC_DIR}/mesh_generator.cpp
    ${SRC_DIR}/camera.cpp
    ${SRC_DIR}/scattering_parameters_window.cpp
    ${SRC_DIR}/fullscreen_window.cpp
    ${SRC_DIR}/scattering_view_window.cpp
    ${SRC_DIR}/gl_application.cpp
    ${SRC_DIR}/shader.cpp
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/shader_library.cpp
    ${SRC_DIR}/mesh.cpp
)

find_package(glfw3 REQUIRED)

set_property(TARGET SubsurfaceScattering PROPERTY CXX_STANDARD 17)
target_link_libraries(imgui PUBLIC glfw)
target_link_libraries(SubsurfaceScattering PRIVATE imgui glad)
target_link_libraries(glad PUBLIC GLESv2 dl)
target_include_directories(glad PUBLIC .)
target_include_directories(imgui PUBLIC ${IMGUI_DIR})
