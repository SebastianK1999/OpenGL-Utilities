#ifndef SPHERE_INCLUDED
#define SPHERE_INCLUDED

#include "oglUtil/drawableSystem/DrawableBase.hpp"

#include <glm/glm.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>


class Sphere : public oglu::DrawableBase {
public:
    glm::vec3 pos;
    glm::vec3 scale;


    Sphere(int N);
    Sphere();
    void setShaders();
    void setBuffers();
    void draw(glm::mat4& MVP);
    void setColor(float r, float g, float b);
 private:
    static std::vector<GLfloat> generateMesh(int N);
    static std::vector<GLfloat> generateNormals(std::vector<GLfloat> points);
    
    const std::vector<GLfloat> g_vertex_buffer_data;
    const std::vector<GLfloat> g_normal_buffer_data;
    const GLfloat* g_color_buffer_data;
    GLfloat cross_color[3] = { 0.0, 1.0, 0.0 };
};

#endif