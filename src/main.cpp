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
#include "inits.h"
#include "object.hpp"
#include "shader.hpp"
#include "entity.hpp"

/*======== GLOBALS ==========================================================*/
GLuint renderingProgram;
std::shared_ptr<OrbitLayer> sun;
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
        100.0f                // far plane
    );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    phongShader = std::make_shared<PhongShader>();
    gouraudShader = std::make_shared<GouraudShader>();
    flatShader = std::make_shared<FlatShader>();

    std::shared_ptr<OrbitLayer> moon = std::make_shared<OrbitLayer>("../models/sphere.obj",flatShader);
    moon->body->setColor(HEX2GL(0x888888));

    std::shared_ptr<OrbitLayer> earth = std::make_shared<OrbitLayer>("../models/sphere.obj",flatShader);
    earth->body->setColor(HEX2GL(0x0000ff));
    earth->below = std::move(moon);

    std::shared_ptr<OrbitLayer> sun = std::make_shared<OrbitLayer>("../models/sphere.obj",flatShader);
    sun->body->setColor(HEX2GL(0xff0000));
    sun->below = std::move(earth);
}
/*===========================================================================*/

/*======== DISPLAY ==========================================================*/
void display(GLFWwindow* window, double currentTime, Camera* cam) {
    /*-------- CLEAR SCREEN -------------------------------------------------*/
    glClearColor(HEX2GLA(0x000000));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*-----------------------------------------------------------------------*/
    sun->draw(cam->lookAt(), proj, l);
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
        glm::vec3(-17.1354, 1.86116, -18.9735),
        glm::vec3(0.635232, 0.305419, 0.709366),
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
