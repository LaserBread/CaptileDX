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

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "animation.hpp"
#include "camera.hpp"
#include "color.h"
#include "control.hpp"
#include "inits.h"
#include "object.hpp"
#include "shader.hpp"

/*======== GLOBALS ==========================================================*/
GLuint renderingProgram;
std::unique_ptr<Object> room;
std::unique_ptr<Object> phongTeapot;
std::unique_ptr<Object> flatTeapot;
std::unique_ptr<Object> gouraudTeapot;
std::unique_ptr<Object> maxwell;
std::unique_ptr<Object> tamosCube;
std::shared_ptr<Texture> tamosTexture;
PointLight l;

std::shared_ptr<PhongShader> phongShader;
std::shared_ptr<GouraudShader> gouraudShader;
std::shared_ptr<FlatShader> flatShader;
ma_engine engine;
/*===========================================================================*/

/*%%%MESS WITH TAMOS%%%*/
void messWithTamos(int j){
    ma_engine_play_sound(&engine,"/home/LaserBread/Music/Soundboard/Fish.mp3", NULL);
    switch(j){
        case 1:
            tamosTexture->setWrapModeS(GL_CLAMP_TO_EDGE);
            std::cout << "Clamped S to edge!\n";
            break;
        case 2:
            tamosTexture->setWrapModeS(GL_CLAMP_TO_BORDER);
            std::cout << "Clamped S to border!\n";
            tamosTexture->setBorderCol(HEX2GL(0xf7d939));
            break;
        case 3:
            tamosTexture->setWrapModeS(GL_REPEAT);
            std::cout << "Tiled along the S!\n";
            break;
        case 4:
            tamosTexture->setWrapModeS(GL_MIRRORED_REPEAT);
            std::cout << "Mirrored along the S!\n";
            break;
        case 5:
            tamosTexture->setWrapModeT(GL_CLAMP_TO_EDGE);
            std::cout << "Clamped T to edge!\n";
            break;
        case 6:
            tamosTexture->setWrapModeT(GL_CLAMP_TO_BORDER);
            std::cout << "Clamped T to border!\n";
            tamosTexture->setBorderCol(HEX2GL(0xf62225));
            break;
        case 7:
            tamosTexture->setWrapModeT(GL_REPEAT);
            std::cout << "Tiled along the T!\n";
            break;
        case 8:
            tamosTexture->setWrapModeT(GL_MIRRORED_REPEAT);
            std::cout << "Mirrored along the T!\n";
            break;
    }
    tamosTexture->updateTex();
}
/*%%%%*/

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

    tamosTexture = std::make_shared<Texture>("../models/tamos.png");
    
    room = std::make_unique<Object>("../models/room.obj",gouraudShader);
    room->setColor(HEX2GL(0x3b4283));

    maxwell = std::make_unique<Object>("../models/maxwell.obj",phongShader, "../models/Maxwell.png");
    std::cout << "piss" <<std::endl;
    maxwell->setColor(HEX2GL(0x888888));
    maxwell->setShininess(30.0f);

    phongTeapot = std::make_unique<Object>("../models/utah_teapot_full.obj",phongShader);
    phongTeapot->setColor(HEX2GL(0xaF0000));
    phongTeapot->setShininess(69.0f);

    gouraudTeapot = std::make_unique<Object>("../models/utah_teapot_full.obj",gouraudShader);
    gouraudTeapot->setColor(HEX2GL(0x00af00));
    gouraudTeapot->setShininess(69.0f);

    flatTeapot = std::make_unique<Object>("../models/utah_teapot_full.obj",flatShader);
    flatTeapot->setColor(HEX2GL(0x0000af));
    flatTeapot->setShininess(69.0f);

    tamosCube = std::make_unique<Object>("../models/tamosCube.obj", phongShader, tamosTexture);

    phongTeapot->setPosition(glm::vec3(
        8.0f * glm::sin(2.0f * glm::pi<float>() *(1.0f/3.0f)),
        0.0f,
        8.0f * glm::cos(2.0f * glm::pi<float>() *(1.0f/3.0f))
    ));
    gouraudTeapot->setPosition(glm::vec3(
        8.0f * glm::sin(2.0f * glm::pi<float>() *(2.0f/3.0f)),
        0,
        8.0f * glm::cos(2.0f * glm::pi<float>() *(2.0f/3.0f))
    ));
    flatTeapot->setPosition(glm::vec3(0.0f,0.0f,8.0f));
    maxwell->scale(1.9f);
    
    room->scale(glm::vec3(20.0f, 15.0f,20.0f));
    room->translate(glm::vec3(0.0f,1.0f, 0.0f));

    tamosCube->scale(2.5);
    tamosCube->translate(glm::vec3(0,7,0));
    tamosCube->setShininess(36);
    

}
/*===========================================================================*/

/*======== DISPLAY ==========================================================*/
void display(GLFWwindow* window, double currentTime, Camera* cam) {
    /*-------- CLEAR SCREEN -------------------------------------------------*/
    glClearColor(HEX2GLA(0x000000));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*-----------------------------------------------------------------------*/
    tamosCube->rotate(delta*20, glm::vec3(0.0f,1.0f,0.0f));

    maxwell->draw(cam->lookAt(),proj,l);
    phongTeapot->draw(cam->lookAt(),proj,l);
    gouraudTeapot->draw(cam->lookAt(),proj,l);
    flatTeapot->draw(cam->lookAt(),proj,l);
    phongTeapot->draw(cam->lookAt(),proj,l);
    room->draw(cam->lookAt(),proj,l);
    tamosCube->draw(cam->lookAt(),proj,l);
}
/*===========================================================================*/

/*======== MAIN =============================================================*/
int main() {
    ma_result result;
    result = ma_engine_init(NULL,&engine);
    if(result != MA_SUCCESS){
        std::cerr << "Failed to initialize audio!\n";
        return -1;
    };

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
    setControls(window, messWithTamos);

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
