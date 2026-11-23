cmake_minimum_required(VERSION 3.27)
project(CaptileDX LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(CAPTILE_FETCH_WINDOWS_DEPS "Fetch Windows-targeted dependencies when building for Windows" ON)

set(CAPTILE_FETCHING_WINDOWS FALSE)
if(WIN32 AND CAPTILE_FETCH_WINDOWS_DEPS)
    set(CAPTILE_FETCHING_WINDOWS TRUE)
endif()

if(CAPTILE_FETCHING_WINDOWS)
    include(FetchContent)

    # Source dependencies are built by the active toolchain (e.g., MinGW64),
    # producing Windows binaries even when cross-compiling from Linux.
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

    set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "" FORCE)
    set(glew-cmake_BUILD_STATIC ON CACHE BOOL "" FORCE)
    set(USE_GLU OFF CACHE BOOL "" FORCE)
    set(ONLY_LIBS ON CACHE BOOL "" FORCE)

    set(SOIL2_BUILD_TESTS OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW TRUE
    )
    FetchContent_Declare(glew
        GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
        GIT_TAG glew-cmake-2.2.0
        GIT_SHALLOW TRUE
    )
    FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
        GIT_SHALLOW TRUE
    )
    FetchContent_Declare(soil2
        GIT_REPOSITORY https://github.com/SpartanJ/SOIL2.git
        GIT_TAG 18c62dfe47ef706f74f1b9791e0155d7debc245d
    )
    FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.8
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(glfw glew glm soil2 imgui)

    if(NOT TARGET GLEW::GLEW)
        if(TARGET libglew_static)
            add_library(GLEW::GLEW ALIAS libglew_static)
        else()
            message(FATAL_ERROR "The fetched GLEW project did not define libglew_static")
        endif()
    endif()

    # The app includes <soil2/SOIL2.h>, while upstream ships SOIL2.h in an
    # uppercase SOIL2 directory. Create a lowercase include shim for Linux hosts
    # doing case-sensitive MinGW cross-compilation.
    set(SOIL2_COMPAT_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated/include")
    file(MAKE_DIRECTORY "${SOIL2_COMPAT_INCLUDE_DIR}/soil2")
    configure_file(
        "${soil2_SOURCE_DIR}/src/SOIL2/SOIL2.h"
        "${SOIL2_COMPAT_INCLUDE_DIR}/soil2/SOIL2.h"
        COPYONLY
    )

    set(SOIL2_INCLUDE_DIR "${SOIL2_COMPAT_INCLUDE_DIR}")
    set(IMGUI_DIR "${imgui_SOURCE_DIR}")

    set(MINIAUDIO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/miniaudio")
else()
    # Native builds can continue using installed libraries and local thirdparty checkouts.
    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(glm CONFIG QUIET)
    set(SOIL2_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")
    set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui")
    set(MINIAUDIO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/miniaudio")
endif()

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

add_library(imgui STATIC
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)
target_include_directories(imgui PUBLIC
    "${IMGUI_DIR}"
    "${IMGUI_DIR}/backends"
)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

add_library(miniaudio STATIC "${MINIAUDIO_DIR}/miniaudio.c")
set_target_properties(miniaudio PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(miniaudio PRIVATE MINIAUDIO_IMPLEMENTATION)
target_include_directories(miniaudio PUBLIC "${MINIAUDIO_DIR}")

# Keep platform-specific target names so Linux and Windows builds stay distinct
# within a single project while producing the same base executable name.
if(WIN32)
    set(CAPTILE_EXECUTABLE_TARGET captile_dx_windows)
else()
    set(CAPTILE_EXECUTABLE_TARGET captile_dx_linux)
endif()

add_executable(${CAPTILE_EXECUTABLE_TARGET}
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
    src/core/audio/audioengine.cpp
    src/core/audio/soundeffect.cpp
    src/core/audio/soundresource.cpp
)
set_target_properties(${CAPTILE_EXECUTABLE_TARGET} PROPERTIES
    OUTPUT_NAME "captile_dx"
)

target_include_directories(${CAPTILE_EXECUTABLE_TARGET} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tinyobjloader"
    "${SOIL2_INCLUDE_DIR}"
)

if(NOT CAPTILE_FETCHING_WINDOWS)
    # Keep support for existing local Linux SOIL2 installations.
    target_include_directories(${CAPTILE_EXECUTABLE_TARGET} PRIVATE /usr/local/include)
    target_link_directories(${CAPTILE_EXECUTABLE_TARGET} PRIVATE /usr/local/lib)
endif()

target_link_libraries(${CAPTILE_EXECUTABLE_TARGET} PRIVATE
    OpenGL::GL
    GLEW::GLEW
    Threads::Threads
    glfw
    soil2
    imgui
    miniaudio
)

if(TARGET glm::glm)
    target_link_libraries(${CAPTILE_EXECUTABLE_TARGET} PRIVATE glm::glm)
endif()

if(MSVC)
    target_compile_options(${CAPTILE_EXECUTABLE_TARGET} PRIVATE /we4715 /we4716)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${CAPTILE_EXECUTABLE_TARGET} PRIVATE -Werror=return-type)
endif()

if(WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_options(${CAPTILE_EXECUTABLE_TARGET} PRIVATE -static)
endif()

# Shader validation uses a host tool, not a Windows-target executable.
find_program(GLSLANG_VALIDATOR NAMES glslangValidator glslang)
if(GLSLANG_VALIDATOR)
    message(STATUS "Found glslangValidator: ${GLSLANG_VALIDATOR}")
    set(SHADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/phong.vert"
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/phong.frag"
    )
    add_custom_target(ValidateShaders ALL
        COMMAND "${GLSLANG_VALIDATOR}" ${SHADER_FILES}
        DEPENDS ${SHADER_FILES}
        COMMENT "Validating GLSL shaders..."
    )
    add_dependencies(${CAPTILE_EXECUTABLE_TARGET} ValidateShaders)
else()
    message(WARNING "glslangValidator not found. Shader build-time syntax checking is disabled.")
endif()