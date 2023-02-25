#ifndef SPHERES_INSTANCES_INCLUDED
#define SPHERES_INSTANCES_INCLUDED

#include "oglUtil/drawableSystem/DrawableBase.hpp"

#include "oglUtil_OLD/Misc.hpp"
#include <glm/glm.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>


class SpheresInstances : public oglu::DrawableBase {
public:
    glm::vec3* pos;
    glm::vec3* scale;
    glm::vec3* rotations; // change to normals

    SpheresInstances(const size_t& N);
    ~SpheresInstances();
    void setShaders();
    void setBuffers();
    void draw(glm::mat4& MVP, const glm::vec3& light);
    void setColor(float r, float g, float b);
    void move(float dt);
    size_t sizeInstances;
    size_t N;
 private:
    static std::vector<GLfloat> generateMesh(int N);
    static std::vector<GLfloat> generateNormals(int N, std::vector<GLfloat> points);
    
    const std::vector<GLfloat> g_vertex_buffer_data;
    const std::vector<GLfloat> g_normal_buffer_data;
    
    GLfloat cross_color[3] = { 0.0, 1.0, 0.0 };
};

#endif