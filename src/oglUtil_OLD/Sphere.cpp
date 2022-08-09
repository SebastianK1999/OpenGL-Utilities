#include "oglUtil_OLD/Sphere.hpp"

#include <vector>
#include <cmath>
#include <math.h>


Sphere::Sphere(int N):
    g_vertex_buffer_data(generateMesh(N)),
    g_normal_buffer_data(generateNormals(g_vertex_buffer_data)),
    pos(0,0,0),
    scale(1,1,1)
{
    //GLfloat cross_color[3] = { 0.0, 1.0, 0.0 };
    setShaders();
    setBuffers();
}

Sphere::Sphere():Sphere(10){}

std::vector<GLfloat> Sphere::generateMesh(int N){

    GLfloat radius = 1;
    // clear memory of prev arrays
    std::vector<GLfloat> vertices;

    GLfloat sectorStep = (GLfloat) M_PI /(GLfloat) N;
    GLfloat stackStep = (GLfloat) M_PI / (GLfloat) N;

    for(int i = 0; i < N; ++i)
    {
        GLfloat stackAngle = i * stackStep - M_PI/2;

        for(int j = 0; j < N*2; ++j)
        {
            GLfloat sectorAngle = j * sectorStep;           // starting from 0 to 2pi
            std::vector<GLfloat> tempVer;

            if(i != 0){
                tempVer.push_back(radius * cosf(stackAngle) * cosf(sectorAngle));
                tempVer.push_back(radius * sinf(stackAngle));
                tempVer.push_back(radius * cosf(stackAngle) * sinf(sectorAngle));

                tempVer.push_back(radius * cosf(stackAngle + stackStep) * cosf(sectorAngle));
                tempVer.push_back(radius * sinf(stackAngle + stackStep));
                tempVer.push_back(radius * cosf(stackAngle + stackStep) * sinf(sectorAngle));

                tempVer.push_back(radius * cosf(stackAngle) * cosf(sectorAngle + sectorStep));
                tempVer.push_back(radius * sinf(stackAngle));
                tempVer.push_back(radius * cosf(stackAngle) * sinf(sectorAngle + sectorStep));
            }
            if(i != (N-1)){
                tempVer.push_back(radius * cosf(stackAngle) * cosf(sectorAngle + sectorStep));
                tempVer.push_back(radius * sinf(stackAngle));
                tempVer.push_back(radius * cosf(stackAngle) * sinf(sectorAngle + sectorStep));

                tempVer.push_back(radius * cosf(stackAngle + stackStep) * cosf(sectorAngle));
                tempVer.push_back(radius * sinf(stackAngle + stackStep));
                tempVer.push_back(radius * cosf(stackAngle + stackStep) * sinf(sectorAngle));

                tempVer.push_back(radius * cosf(stackAngle + stackStep) * cosf(sectorAngle + sectorStep));
                tempVer.push_back(radius * sinf(stackAngle + stackStep));
                tempVer.push_back(radius * cosf(stackAngle + stackStep) * sinf(sectorAngle + sectorStep));
            }

            vertices.insert(vertices.end(), tempVer.begin(), tempVer.end());
        }
    }

    return vertices;
}

std::vector<GLfloat> Sphere::generateNormals(std::vector<GLfloat> points){
    std::vector<GLfloat> normals;
    normals.reserve(points.size());
    for(int i = 0; i < points.size(); i += 9)
    {
        glm::vec3 p1(
            points[i],
            points[i+1],
            points[i+2]
        );
        glm::vec3 p2(
            points[i+3],
            points[i+4],
            points[i+5]
        );
        glm::vec3 p3(
            points[i+6],
            points[i+7],
            points[i+8]
        );

        /*
        glm::vec3 V = (p1-p2);
        glm::vec3 U = (p3-p2);

        glm::vec3 Norm;
        Norm.x =  U.y*V.z - U.z*V.y;
        Norm.y =  U.z*V.x - U.x*V.z;
        Norm.z =  U.x*V.y - U.y*V.x;
        
        GLfloat len = std::sqrt(Norm.x*Norm.x + Norm.y*Norm.y + Norm.z*Norm.z);
        Norm.x = Norm.x / len;
        Norm.y = Norm.y / len;
        Norm.z = Norm.z / len;
        */


        glm::vec3 Norm = p1;
        GLfloat len = std::sqrt(Norm.x*Norm.x + Norm.y*Norm.y + Norm.z*Norm.z);
        Norm.x = Norm.x / len;
        Norm.y = Norm.y / len;
        Norm.z = Norm.z / len;
       
        normals.emplace_back(Norm.x),
        normals.emplace_back(Norm.y),
        normals.emplace_back(Norm.z),
        
        Norm = p2;
        len = std::sqrt(Norm.x*Norm.x + Norm.y*Norm.y + Norm.z*Norm.z);
        Norm.x = Norm.x / len;
        Norm.y = Norm.y / len;
        Norm.z = Norm.z / len;
        
        normals.emplace_back(Norm.x);
        normals.emplace_back(Norm.y);
        normals.emplace_back(Norm.z);

        Norm = p3;
        len = std::sqrt(Norm.x*Norm.x + Norm.y*Norm.y + Norm.z*Norm.z);
        Norm.x = Norm.x / len;
        Norm.y = Norm.y / len;
        Norm.z = Norm.z / len;
        
        normals.emplace_back(Norm.x);
        normals.emplace_back(Norm.y);
        normals.emplace_back(Norm.z);
    }
    return normals;
}

void Sphere::setShaders() {
    compileShaders(R"END(

        #version 330 core
        #extension GL_ARB_explicit_uniform_location : require
        #extension GL_ARB_shading_language_420pack : require

        layout(location = 0) in vec3 vertexPosition;
        layout(location = 1) in vec3 normal;
        layout(location = 0) uniform vec4 pos;
        layout(location = 1) uniform vec4 scale;
        layout(location = 2) uniform vec3 light;

        layout(location = 3)uniform mat4 MVP;

        out vec3 vPos;
        out vec3 vCol;

        void main(){
            vec3 pos3 = vec3(pos[0],pos[1],pos[2]);
            float R = length(pos3 - light);
            vec3 sc = vec3(scale[0],scale[1],scale[2]);
            vec3 L = (pos3 - light)/R;	
            
            gl_Position =  MVP * (vec4(vertexPosition,1) * scale + pos);

            float r = R;
            if(r > 1){
                r = 1.0;
            }

            if( acos(dot(normal, L) / length(normal) / length(L)) > 3.141593/2.0){
                vCol = vec3(0,0.6,0.9) * (length(dot(normal,L)));
            }
            else{
                vCol = vec3(1,1,1) * 0.7 * (length(dot(normal,L)));
            }
            vPos = pos3 + sc * vertexPosition;
        }
    )END", R"END(

        #version 330 core

        in vec3 vPos;
        in vec3 vCol;

        out vec4 color;

        void main(){
            vec3 objCol = vec3(0.5,0,0.5);
            objCol = objCol*((vPos[1]+1) * 0.5);

            // alpha blending
            float I = length(vCol) + length(objCol)*(1.0 - length(vCol));
            objCol = ( vCol*length(vCol) + objCol*length(objCol) * (1.0 - length(vCol)) ) / I;

            objCol = objCol*((vPos[1]+1) * 0.9);

            color = vec4(
                objCol[0],
                objCol[1],
                objCol[2],
                (0.25 + 0.6 * length(vCol))
            );

            //color *= ((vPos[1]+1) * 1);

        }
    )END");
}

void Sphere::setBuffers() {
    bindBuffers();

    GLuint vertexBuffer;    
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * g_vertex_buffer_data.size(), g_vertex_buffer_data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    //GLuint normalBuffer;   
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * g_normal_buffer_data.size(), g_normal_buffer_data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
        1,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
}

void Sphere::draw(glm::mat4& MVP) {
    bindProgram();
    bindBuffers();
    glUniform4f(0, pos[0],pos[1],pos[2],0);
    glUniform4f(1, scale[0],scale[1],scale[2],1);
    glUniform3f(2, 0.0f, 1.0f, 2.0f);
    glUniformMatrix4fv(3, 1, GL_FALSE, &MVP[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, g_vertex_buffer_data.size()/3); // 12*3 indices starting at 0 -> 12 triangles
}

void Sphere::setColor(float r, float g, float b){
    cross_color[0]=r;cross_color[1]=g;cross_color[2]=b;
}