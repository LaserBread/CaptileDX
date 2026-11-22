/* CONTROL (Source) - Ethan Jones - re
 * DESCRIPTION:
 *
 */

/*======== HEADERS ==========================================================*/
#include "control.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <soil2/SOIL2.h>  // For SOIL2
#include <stdio.h>

#include <cmath>
#include <fstream>
#include <glm/glm.hpp>  // Basic include for GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "camera.hpp"
#include "inits.h"
/*===========================================================================*/

#define T_ACT "\x1b[30;44;1m"
#define T_R "\x1b[0m"

/*======== GLOBALS ==========================================================*/
bool paused = false;
bool pickedUpLight = false;
double pauseStart = 0.0f;
double pauseTotal = 0.0f;
bool mouseLookEnabled = false;
Camera* cam = nullptr;
bool adjustButtonPressed = false;
void (*callback)(int);
/*===========================================================================*/

/*======== PAUSE ANIMATION ==================================================*/
void pause() {
    if (!paused) {
        pauseStart = glfwGetTime();
        paused = true;
        std::cout << T_ACT << "Paused at " << pauseStart << T_R << std::endl;
    } else {
        paused = false;
        pauseTotal += glfwGetTime() - pauseStart;
        std::cout << T_ACT << "Unpaused at " << pauseTotal << T_R << std::endl;
    }
};
/*===========================================================================*/

/*======== KEY DISPATCH =====================================================*/
// Runs when a key is pressed.
void eventDispatch(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods) {
    if (action == GLFW_PRESS)
        switch (key) {
            /*%%PAUSE ANIMATION%%*/
            case GLFW_KEY_P:
                pause();
                break;
            /*%%TOGGLE MOUSE LOOK%%*/
            case GLFW_KEY_M:
                mouseLookEnabled = !mouseLookEnabled;
                if (mouseLookEnabled) {
                    std::cout << "Mouse-look: ENABLED" << std::endl;
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    firstMouse = true;  // prevent jump when re-enabling
                } else {
                    std::cout << "Mouse-look: DISABLED" << std::endl;
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
                break;
            /*%%PICK UP/DROP LIGHT%%*/
            case GLFW_KEY_E:
                pickedUpLight = !pickedUpLight;
                if (pickedUpLight) {
                    std::cout << "Picked Up Light" << std::endl;
                } else {
                    std::cout << "Dropped Light" << std::endl;
                }
                break;
            /*%%DUMP POSITION AND FACING%%*/
            case GLFW_KEY_X:
                std::cout << "Position: " << cam->getPos().x << ", " << cam->getPos().y << ", " << cam->getPos().z << std::endl
                          << "Facing: " << cam->getDirect().x << ", " << cam->getDirect().y << ", " << cam->getDirect().z << std::endl;
                break;
            /*%%CHANGE TEXTURE MODE%%*/
            case GLFW_KEY_1:
                callback(1);
                break;
            case GLFW_KEY_2:
                callback(2);
                break;
            case GLFW_KEY_3:
                callback(3);
                break;
            case GLFW_KEY_4:
                callback(4);
                break;
            case GLFW_KEY_5:
                callback(5);
                break;
            case GLFW_KEY_6:
                callback(6);
                break;
            case GLFW_KEY_7:
                callback(7);
                break;
            case GLFW_KEY_8:
                callback(8);
                break;
            /*%%%%*/
        }
}
/*===========================================================================*/

/*======== MOUSE CALLBACK ===================================================*/
// Occurs when the mouse is called.
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    if (!mouseLookEnabled) {
        return;
    }

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;  // reversed

    lastX = xpos;
    lastY = ypos;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;
    cam->rotate(xoffset, yoffset);
}

/*======== FRAME DISPATCH ===================================================*/
// Stuff to be run every frame.
void frameDispatch(GLFWwindow* window, float delta) {
    /*-------- SPRINT WHEN SHIFT IS HELD ------------------------------------*/
    cam->speed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? 6.0f : 2.5f;
    /*-----------------------------------------------------------------------*/

    /*-------- MOVE WITH WASD -----------------------------------------------*/
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam->pos += delta * cam->speed * cam->direct;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam->pos -= delta * cam->speed * cam->direct;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam->pos -= glm::normalize(glm::cross(cam->direct, cam->up)) * cam->speed * delta;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam->pos += glm::normalize(glm::cross(cam->direct, cam->up)) * cam->speed * delta;
    }
    /*-----------------------------------------------------------------------*/
}
/*===========================================================================*/

/*======== SETUP CONTROLS ===================================================*/
// Sets up controls
void setControls(GLFWwindow* window, void (*incallback)(int)) {
    callback = incallback;
    glfwSetKeyCallback(window, eventDispatch);
    glfwSetCursorPosCallback(window, mouse_callback);
}
/*===========================================================================*/

/*======== SETUP CAMERA =====================================================*/
// Pass a camera in to contorl.
void setCamera(Camera* inCam) {
    cam = inCam;
}
/*===========================================================================*/

/*======== GET TIME =========================================================*/
// Gets the current animation time, compensated for pauses.
double getTime() {
    assert(!paused);  // DO NOT CALL THIS FUNCTION WHILE PAUSED!

    assert(glfwGetTime() - pauseTotal >= 0);  // WHY IS THE TOTAL PAUSE TIME LONGER THAN THE PROGRAM HAS BEEN RUNNING?

    /*-------- UNPAUSED, SEND PAUSE-COMPENSATED TIME ------------------------*/
    return glfwGetTime() - pauseTotal;
    /*-----------------------------------------------------------------------*/
}
/*===========================================================================*/
