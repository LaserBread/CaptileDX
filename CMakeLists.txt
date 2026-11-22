cmake_minimum_required(VERSION 3.10)
project(OpenGLTest CXX)

set(CMAKE_CXX_STANDARD 17)

# Find system libraries
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Threads REQUIRED)

set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui")

add_library(imgui STATIC
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    # Optional: include if you use ImGui's C++ STL wrappers or misc helpers
    # "${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp"
    # Backends (choose what your app uses)
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

target_include_directories(imgui PUBLIC "${IMGUI_DIR}")


set(MINIAUDIO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/miniaudio")


add_library(miniaudio STATIC
    "${MINIAUDIO_DIR}/miniaudio.c"
)
set_target_properties(miniaudio PROPERTIES
    LINKER_LANGUAGE CXX
)
target_compile_definitions(miniaudio PRIVATE MINIAUDIO_IMPLEMENTATION)
target_include_directories(miniaudio PUBLIC "${MINIAUDIO_DIR}")

add_executable(
    captile_dx
    src/main.cpp 
    src/core/inits.cpp
    src/object/animation.cpp
    src/core/control.cpp
    src/render/camera.cpp
    src/render/shading/shader.cpp
    src/render/shading/litshader.cpp
    src/object/object.cpp
    src/render/light.cpp
    src/render/texture.cpp
    src/render/shading/phong.cpp
    src/render/shading/flat.cpp
    src/render/shading/gouraud.cpp
)

# Tell the compiler where to find your header files
target_include_directories(captile_dx PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tinyobjloader
)

# Include custom local path if SOIL2 was installed under /usr/local
target_include_directories(captile_dx PRIVATE /usr/local/include)
target_link_directories(captile_dx PRIVATE /usr/local/lib)

# Link libraries to the executable
target_link_libraries(captile_dx PRIVATE
    OpenGL::GL
    GLEW::GLEW
    Threads::Threads
    glfw
    soil2
    imgui
    miniaudio
)

if(MSVC)
    target_compile_options(captile_dx PRIVATE /we4715 /we4716)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(captile_dx PRIVATE -Werror=return-type)
endif()

# ------------------------------------------------------------------------------
# Shader Validation Step via glslangValidator
# ------------------------------------------------------------------------------
find_program(GLSLANG_VALIDATOR NAMES glslangValidator glslang)

if(GLSLANG_VALIDATOR)
    message(STATUS "Found glslangValidator: ${GLSLANG_VALIDATOR}")

    # List GLSL shader files to check
    set(SHADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/phong.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders/phong.frag
    )

    # Add custom target that validates shaders during every build
    add_custom_target(ValidateShaders ALL
        COMMAND ${GLSLANG_VALIDATOR} ${SHADER_FILES}
        DEPENDS ${SHADER_FILES}
        COMMENT "Validating GLSL shaders..."
        )

    # Ensure shaders are validated before building the executable
    add_dependencies(captile_dx ValidateShaders)
else()
    message(WARNING "glslangValidator not found. Shader build-time syntax checking is disabled.")
endif()