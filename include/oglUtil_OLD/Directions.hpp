#ifndef DIRECTIONS_INCLUDED
#define DIRECTIONS_INCLUDED

#include "oglUtil/drawables/Drawable.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>


class Directions : public oglu::Drawable {
public:
    Directions();
    void setShaders();
    void setBuffers();
    void draw(const glm::mat4& MVP);
 private:
    static const GLfloat g_vertex_buffer_data[];
    static const GLfloat g_color_buffer_data[];
};

#endif