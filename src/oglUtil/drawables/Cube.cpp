/*
* MIT License
* 
* Copyright (c) 20233 Sebastian Kwaśniak
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

#include "oglUtil/drawableSystem/DrawableRegistry.hpp"
#include "oglUtil/drawableSystem/ShaderCollection.hpp"

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

std::shared_ptr<oglu::RegisteredDrawable> oglu::Cube::registryPointer = nullptr;

void oglu::Cube::drawInstances(const glm::mat4& MVP, const glm::vec3& light)
{
    registryPointer->drawInstances(MVP, light);
}

oglu::RegisteredDrawable& oglu::Cube::getRegistry() const noexcept 
{
    return *(registryPointer);
}

oglu::Cube::Cube()
{
    if(registryPointer == nullptr)
    {
        registryPointer = oglu::DrawableRegistry::registerDrawable
        (
            oglu::Cube::key,
            gs_VertexBuffer,
            std::vector<glm::vec4>(gs_VertexBuffer.size(),  {1.0f,1.0f,1.0f,1.0f}),
            gs_NormalBuffer,
            std::vector<glm::vec2>(),
            std::vector<unsigned char>(),
            0,
            0,
            oglu::ShaderCollection::basicVertexShader,
            oglu::ShaderCollection::basicFragmentShader
        );
    }
    instancePointer = registryPointer->addInstance();
}

