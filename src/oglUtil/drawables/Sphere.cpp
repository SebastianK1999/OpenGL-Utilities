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

#include "oglUtil/drawables/Sphere.hpp"

std::shared_ptr<oglu::RegisteredDrawable> oglu::Sphere::registryPointer = nullptr;
unsigned int oglu::Sphere::levelOfDetail = 10;

void oglu::Sphere::drawInstances(const glm::mat4& MVP, const glm::vec3& light)
{
    registryPointer->drawInstances(MVP, light);
}

oglu::RegisteredDrawable& oglu::Sphere::getRegistry() const noexcept 
{
    return *(registryPointer);
}

oglu::Sphere::Sphere()
{
    if(registryPointer == nullptr)
    {
        std::vector<glm::vec3> vertexBuffer = oglu::Sphere::generateMesh();
        std::vector<glm::vec3> normalBuffer = oglu::Sphere::generateNormals(vertexBuffer);
        registryPointer = oglu::DrawableRegistry::registerDrawable
        (
            oglu::Sphere::key,
            vertexBuffer,
            std::vector<glm::vec4>(vertexBuffer.size(),  {1.0f,1.0f,1.0f,1.0f}),
            normalBuffer,
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

const std::vector<glm::vec3> oglu::Sphere::generateMesh()
{
    const GLfloat radius = 1;
    const GLfloat sectorStep = (GLfloat) M_PI /(GLfloat) Sphere::levelOfDetail;
    const GLfloat stackStep = (GLfloat) M_PI / (GLfloat) Sphere::levelOfDetail;
    std::vector<glm::vec3> vertices;
    vertices.reserve(2 * (Sphere::levelOfDetail-1) * 3 * 2 * Sphere::levelOfDetail);

    for(int i = 0; i < Sphere::levelOfDetail; ++i)
    {
        GLfloat stackAngle = i * stackStep - M_PI/2.0f;
        for(int j = 0; j < Sphere::levelOfDetail*2; ++j)
        {
            GLfloat sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            if(i != 0){
                vertices.emplace_back
                (glm::vec3(
                    radius * cosf(stackAngle) * cosf(sectorAngle),
                    radius * sinf(stackAngle),
                    radius * cosf(stackAngle) * sinf(sectorAngle)
                ));

                vertices.emplace_back
                (glm::vec3(
                    radius * cosf(stackAngle + stackStep) * cosf(sectorAngle),
                    radius * sinf(stackAngle + stackStep),
                    radius * cosf(stackAngle + stackStep) * sinf(sectorAngle)
                ));

                vertices.emplace_back
                (glm::vec3(
                    radius * cosf(stackAngle) * cosf(sectorAngle + sectorStep),
                    radius * sinf(stackAngle),
                    radius * cosf(stackAngle) * sinf(sectorAngle + sectorStep)
                ));
            }
            if(i != (Sphere::levelOfDetail-1)){
                vertices.emplace_back
                (glm::vec3(
                    radius * cosf(stackAngle) * cosf(sectorAngle + sectorStep),
                    radius * sinf(stackAngle),
                    radius * cosf(stackAngle) * sinf(sectorAngle + sectorStep)
                ));

                vertices.emplace_back
                (glm::vec3(
                    radius * cosf(stackAngle + stackStep) * cosf(sectorAngle),
                    radius * sinf(stackAngle + stackStep),
                    radius * cosf(stackAngle + stackStep) * sinf(sectorAngle)
                ));

                vertices.emplace_back
                (glm::vec3(
                    radius * cosf(stackAngle + stackStep) * cosf(sectorAngle + sectorStep),
                    radius * sinf(stackAngle + stackStep),
                    radius * cosf(stackAngle + stackStep) * sinf(sectorAngle + sectorStep)
                ));
            }
        }
    }
    return vertices;
}

const std::vector<glm::vec3> oglu::Sphere::generateNormals(const std::vector<glm::vec3>& vertexBuffer){
    std::vector<glm::vec3> normals;
    normals.reserve(vertexBuffer.size());
    for(int i = 0; i < vertexBuffer.size(); i++)
    {
        normals.emplace_back(glm::normalize(vertexBuffer[i]));
    }
    return normals;
}