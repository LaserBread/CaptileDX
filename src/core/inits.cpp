#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <soil2/SOIL2.h>  // For SOIL2
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <glm/glm.hpp>  // Basic include for GLM
#include <iostream>
#include <string>
#include "inits.h"

GLint mLoc = -1;     // Stores the m location
GLint vLoc = -1;    // Stores the v location
GLint pLoc = -1;  // Stores the color location

glm::mat4 proj;
int windowWd = 800;
int windowHt = 600;
float aspect = (float)windowWd / (float)windowHt;
float delta = 0.0f;
float lastFrame = 0.0f;
float lastX = (float)windowWd / 2.0f; // centered horizontally
float lastY = (float)windowHt / 2.0f; // centered vertically
bool firstMouse = true;

/*======== START GLEW =======================================================*/
// Relocated the long GLEW startup stuff over here.
int startGlew() {
    glewExperimental = GL_TRUE;  // Ensures all extensions are loaded including Core Profiles
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        const GLubyte* version = glGetString(GL_VERSION);
        if (version) {
            std::cout << "GLEW reported an error during init, but OpenGL context is valid!" << std::endl;
            std::cout << "OpenGL Version: " << version << std::endl;
        } else {
            std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
            return -1;
        }
    }
    glGetError();
    return 0;
}
/*===========================================================================*/

/*======== LOAD SHADER SOURCE ===============================================*/
// Loads a shader and returns its contents. Returns an empty string on failure.
std::string loadShaderSource(const char* filePath) {
    std::string source;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return "";
    } else {
        std::cout << "Loading shader: " << filePath << std::endl;
    }

    std::string line;
    while (std::getline(fileStream, line)) {
        source.append(line + "\n");
    }

    fileStream.close();
    return source;
}
/*===========================================================================*/

/*======== WHEN WINDOW RESIZED ==============================================*/
// Callback that runs when a window is resized.
void whenWindowResized(GLFWwindow* window, int width, int height) {
    // Avoid a divide-by-zero error if the window is minimized
    if (height == 0) {
        return;
    }

    // Update all our window size globals
    windowWd = width;
    windowHt = height;
    aspect = (float)width / (float)height;

    // Tell OpenGL the new dimensions
    glViewport(0, 0, width, height);

    // Calculate our new projection matrix
    proj = glm::perspective(
        glm::radians(45.0f),  // FOV
        aspect,               // aspect ratio (hard-coded for now)
        0.1f,                 // near plane
        100.0f                // far plane
    );
}
/*===========================================================================*/
