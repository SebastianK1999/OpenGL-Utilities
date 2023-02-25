/*
* MIT License
* 
* Copyright (c) 20223 Sebastian Kwaśniak
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/


#include <vector>

#include "oglUtil/tools/OgluFunctions.hpp"
#include "oglUtil/drawableSystem/DrawableRegistry.hpp"

#include "oglUtil/drawables/Cube.hpp"

static const std::vector<glm::vec3> gs_VertexBuffer = { 
    glm::vec3( 0.5f, 0.5f,-0.5f),
    glm::vec3( 0.5f,-0.5f,-0.5f),
    glm::vec3(-0.5f, 0.5f,-0.5f),

    glm::vec3(-0.5f,-0.5f,-0.5f),
    glm::vec3(-0.5f, 0.5f,-0.5f),
    glm::vec3( 0.5f,-0.5f,-0.5f),

    glm::vec3( 0.5f, 0.5f, 0.5f),
    glm::vec3(-0.5f, 0.5f, 0.5f),
    glm::vec3( 0.5f,-0.5f, 0.5f),

    glm::vec3(-0.5f,-0.5f, 0.5f),
    glm::vec3( 0.5f,-0.5f, 0.5f),
    glm::vec3(-0.5f, 0.5f, 0.5f),


    glm::vec3( 0.5f,-0.5f, 0.5f),
    glm::vec3(-0.5f,-0.5f, 0.5f),
    glm::vec3( 0.5f,-0.5f,-0.5f),

    glm::vec3(-0.5f,-0.5f,-0.5f),
    glm::vec3( 0.5f,-0.5f,-0.5f),
    glm::vec3(-0.5f,-0.5f, 0.5f),

    glm::vec3( 0.5f, 0.5f, 0.5f),
    glm::vec3( 0.5f, 0.5f,-0.5f),
    glm::vec3(-0.5f, 0.5f, 0.5f),

    glm::vec3(-0.5f, 0.5f,-0.5f),
    glm::vec3(-0.5f, 0.5f, 0.5f),
    glm::vec3( 0.5f, 0.5f,-0.5f),


    glm::vec3(-0.5f, 0.5f, 0.5f),
    glm::vec3(-0.5f, 0.5f,-0.5f),
    glm::vec3(-0.5f,-0.5f, 0.5f),

    glm::vec3(-0.5f,-0.5f,-0.5f),
    glm::vec3(-0.5f,-0.5f, 0.5f),
    glm::vec3(-0.5f, 0.5f,-0.5f),

    glm::vec3( 0.5f, 0.5f, 0.5f),
    glm::vec3( 0.5f,-0.5f, 0.5f),
    glm::vec3( 0.5f, 0.5f,-0.5f),

    glm::vec3( 0.5f,-0.5f,-0.5f),
    glm::vec3( 0.5f, 0.5f,-0.5f),
    glm::vec3( 0.5f,-0.5f, 0.5f),
};

static const std::vector<glm::vec3> gs_NormalBuffer = { 
    glm::vec3(0.0f, 0.0f,-1.0f),
    glm::vec3(0.0f, 0.0f,-1.0f),
    glm::vec3(0.0f, 0.0f,-1.0f),

    glm::vec3(0.0f, 0.0f,-1.0f),
    glm::vec3(0.0f, 0.0f,-1.0f),
    glm::vec3(0.0f, 0.0f,-1.0f),

    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),

    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    

    glm::vec3(0.0f,-1.0f, 0.0f),
    glm::vec3(0.0f,-1.0f, 0.0f),
    glm::vec3(0.0f,-1.0f, 0.0f),

    glm::vec3(0.0f,-1.0f, 0.0f),
    glm::vec3(0.0f,-1.0f, 0.0f),
    glm::vec3(0.0f,-1.0f, 0.0f),

    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),

    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),


    glm::vec3(-1.0f,0.0f, 0.0f),
    glm::vec3(-1.0f,0.0f, 0.0f),
    glm::vec3(-1.0f,0.0f, 0.0f),

    glm::vec3(-1.0f,0.0f, 0.0f),
    glm::vec3(-1.0f,0.0f, 0.0f),
    glm::vec3(-1.0f,0.0f, 0.0f),

    glm::vec3( 1.0f,0.0f, 0.0f),
    glm::vec3( 1.0f,0.0f, 0.0f),
    glm::vec3( 1.0f,0.0f, 0.0f),

    glm::vec3( 1.0f,0.0f, 0.0f),
    glm::vec3( 1.0f,0.0f, 0.0f),
    glm::vec3( 1.0f,0.0f, 0.0f),

};

static const std::string gs_VertexShader = R"END(
    #version 330 core
    #extension GL_ARB_explicit_uniform_location : require
    #extension GL_ARB_shading_language_420pack : require

    layout(location = 0) in vec3 vVertex;
    layout(location = 1) in vec4 vVertexColor;
    layout(location = 2) in vec3 vNormal;

    layout(location = 3) in vec3 vPosition;
    layout(location = 4) in vec3 vScale;
    layout(location = 5) in vec3 vRotation;
    layout(location = 6) in vec4 vColor;

    layout(location = 0)uniform mat4 vMVP;
    layout(location = 1)uniform vec3 vLight;

    out vec3 fPosition;
    out vec3 fNormal;
    out vec3 fLight;
    out vec4 fColor;

    vec3 rotate(vec3 vertex, vec3 rotations)
    {
        vec3 result;
        float cosX = cos(rotations.x);
        float sinX = sin(rotations.x);
        float cosY = cos(rotations.y);
        float sinY = sin(rotations.y);
        float cosZ = cos(rotations.z);
        float sinZ = sin(rotations.z);
        
        mat3 rotationMatrix = mat3(
            cosY * cosZ,
            -cosX * sinZ + sinX * sinY * cosZ,
            sinX * sinZ + cosX * sinY * cosZ,
            cosY * sinZ,
            cosX * cosZ + sinX * sinY * sinZ,
            -sinX * cosZ + cosX * sinY * sinZ,
            -sinY,
            sinX * cosY,
            cosX * cosY
        );
        
        result = rotationMatrix * vertex;
        
        return result;
    }

    void main()
    {	
        vec3 vertex;

        vertex = vVertex * vScale;
        vertex = rotate(vertex, vRotation);
        vertex = vertex + vPosition;

        gl_Position =  vMVP * vec4(vertex,1);
        fPosition = vertex;
        fNormal = vNormal;
        fLight = vLight;
        fColor = vVertexColor;
    }

)END";

static const std::string gs_FragmentShader = R"END(

    #version 330 core

    in vec3 fPosition;
    in vec3 fNormal;
    in vec3 fLight;
    in vec4 fColor;

    out vec4 Color;

    void main()
    {
        vec3 light = normalize(fLight - fPosition);
        vec3 normal = normalize(fNormal);
        float shading = clamp(dot(normal,light),0,1);
        Color = fColor * shading * 0.7 + 0.3;
        Color.a = fColor.a;
    }

)END";

std::shared_ptr<oglu::RegisteredDrawable> oglu::Cube::registryPointer = nullptr;

oglu::RegisteredDrawable& oglu::Cube::getRegistry() const noexcept 
{
    return *(registryPointer);
}

oglu::Cube::Cube()
{
    oglu::printErrors("construct cube 0");
    if(registryPointer == nullptr)
    {
        registryPointer = oglu::DrawableRegistry::registerDrawable
        (
            oglu::Cube::key,
            gs_VertexBuffer,
            std::vector<glm::vec4>(gs_VertexBuffer.size(),  {1.0f,1.0f,1.0f,1.0f}),
            gs_NormalBuffer,
            gs_VertexShader,
            gs_FragmentShader
        );
    }
    instancePointer = registryPointer->addInstance();
}

