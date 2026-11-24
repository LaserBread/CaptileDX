#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <soil2/SOIL2.h>  // For SOIL2

#include <cmath>
#include <fstream>
#include <glm/glm.hpp>  // Basic include for GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "animation.hpp"
#include "audioengine.hpp"
#include "camera.hpp"
#include "color.h"
#include "control.hpp"
#include "entity.hpp"
#include "inits.h"
#include "object.hpp"
#include "shader.hpp"

/*======== GLOBALS ==========================================================*/
GLuint renderingProgram;
std::shared_ptr<OrbitLayer> base;
PointLight l;

std::shared_ptr<PhongShader> phongShader;
std::shared_ptr<GouraudShader> gouraudShader;
std::shared_ptr<FlatShader> flatShader;
/*===========================================================================*/

/*======== INITIALIZE OPENGL ================================================*/
void init(GLFWwindow* window) {
    proj = glm::perspective(
        glm::radians(45.0f),  // FOV
        aspect,               // aspect ratio (hard-coded for now)
        0.1f,                 // near plane
        1000.0f               // far plane
    );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    phongShader = std::make_shared<PhongShader>();
    gouraudShader = std::make_shared<GouraudShader>();
    flatShader = std::make_shared<FlatShader>();

    std::shared_ptr<Texture> rowboat = std::make_shared<Texture>("../models/rowboat.jpg");
    base = std::make_shared<OrbitLayer>("../models/rowboat.obj", phongShader, rowboat);
    base->body->setShininess(70.0f);

    std::shared_ptr<Texture> fish = std::make_shared<Texture>("../models/clownfish.jpg");
    std::shared_ptr<OrbitLayer> clownfish = std::make_shared<OrbitLayer>("../models/clownfish.obj", phongShader, fish);
    clownfish->body->setShininess(60.0f);
    base->below
}
/*===========================================================================*/

/*======== DISPLAY ==========================================================*/
void display(GLFWwindow* window, double currentTime, Camera* cam) {
    /*-------- CLEAR SCREEN -------------------------------------------------*/
    glClearColor(HEX2GLA(0x000000));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*-----------------------------------------------------------------------*/
    base->draw(cam->lookAt(), proj, l);
}
/*===========================================================================*/

/*======== MAIN =============================================================*/
int main() {
    std::cout << "Starting up\n";
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    Camera cam = Camera(
        glm::vec3(-11.1947, 37.4055, -11.5315),
        glm::vec3(0.218092, -0.872161, 0.437916),
        glm::vec3(0.0f, 1.0f, 0.0f),
        0.5f);

    /*-------- CREATE GLFW WINDOW -------------------------------------------*/
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // Needed for MacOS`
    GLFWwindow* window = glfwCreateWindow(windowWd, windowHt, "OpenGL Test", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        return -1;
    }
    /*-----------------------------------------------------------------------*/

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, whenWindowResized);
    setCamera(&cam);
    setControls(window);

    if (startGlew() != 0) return 1;  // Try to start GLEW
    glfwSwapInterval(1);             // enables Vsync3

    init(window);
    /*-------- RUN PROGRAM --------------------------------------------------*/
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        delta = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (!paused) {
            if (pickedUpLight) l.setPos(cam.getPos());
            frameDispatch(window, delta);
            display(window, getTime(), &cam);
            glfwSwapBuffers(window);
        }
        glfwPollEvents();
    }
    /*-----------------------------------------------------------------------*/

    /*-------- CLOSE PROGRAM ------------------------------------------------*/
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
    /*-----------------------------------------------------------------------*/
}
/*===========================================================================*/
