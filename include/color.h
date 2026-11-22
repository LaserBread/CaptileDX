#ifndef TYPES_H
#define TYPES_H
#include <glm/gtc/type_ptr.hpp>

// Convert a hex color code to OpenGL RGB values with the format 0xRRGGBB
#define HEX2GL(hex)                   \
    ((hex >> 16) & 0xFF) / 255.0f,    \
        ((hex >> 8) & 0xFF) / 255.0f, \
        (hex & 0xFF) / 255.0f

#define HEX2GLA(hex)                   \
    ((hex >> 16) & 0xFF) / 255.0f,    \
        ((hex >> 8) & 0xFF) / 255.0f, \
        (hex & 0xFF) / 255.0f,        \
        1.0f


#define COL_WHITE glm::vec3(1.0f,1.0f,1.0f);
#define COL_BLACK glm::vec3(0.0f,0.0f,0.0f);
#define COL_RED glm::vec3(HEX2GL(0xFF0000));
#define COL_GREEN glm::vec3(HEX2GL(0x00FF00));
#define COL_BLUE glm::vec3(HEX2GL(0x0000FF));
#endif