/*
* MIT License
* 
* Copyright (c) 2023 Sebastian Kwaśniak
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

#include <utility>
#include <iostream>

#include "oglUtil/tools/OgluFunctions.hpp"

#include "oglUtil/drawableSystem/RegisteredDrawable.hpp"

std::shared_ptr<oglu::DrawableInstancePointers> oglu::RegisteredDrawable::addInstance()
{
    instancePositions.emplace_back  (glm::vec3(0.0f,0.0f,0.0f));
    instanceScales.emplace_back     (glm::vec3(1.0f,1.0f,1.0f));
    instanceRotations.emplace_back  (glm::vec3(0.0f,0.0f,0.0f));
    instanceColors.emplace_back     (glm::vec4(1.0f,1.0f,1.0f,1.0f));
    instances.emplace_back(
        new oglu::DrawableInstancePointers(
            &instancePositions.back(),
            &instanceScales.back(),
            &instanceRotations.back(),
            &instanceColors.back()
        )
    );
    return instances.back();
}

void oglu::RegisteredDrawable::reserve(const unsigned int numberOfNewInstances)
{
    instancePositions.reserve(instancePositions.capacity() + numberOfNewInstances);
    instanceScales.reserve(instanceScales.capacity() + numberOfNewInstances);
    instanceRotations.reserve(instanceRotations.capacity() + numberOfNewInstances);
    instanceColors.reserve(instanceColors.capacity() + numberOfNewInstances);
    instances.reserve(instances.capacity() + numberOfNewInstances);
}

oglu::RegisteredDrawable::RegisteredDrawable(
    const std::string& _key,
    const std::vector<glm::vec3> _vertexBuffer,
    const std::vector<glm::vec4> _vertexColorBuffer,
    const std::vector<glm::vec3> _normalBuffer,
    const std::string& _vertexShaderCode,
    const std::string& _fragmentShaderCode
)
: oglu::DrawableBase()
, key(_key)
, vertexBuffer(_vertexBuffer)
, vertexColorBuffer(_vertexColorBuffer)
, normalBuffer(_normalBuffer)
, vertexShaderCode(_vertexShaderCode)
, fragmentShaderCode(_fragmentShaderCode)
{
    setShaders();
    setBuffers();
}

void oglu::RegisteredDrawable::setShaders()
{
    compileShaders(vertexShaderCode.c_str(), fragmentShaderCode.c_str());
}

void oglu::RegisteredDrawable::setBuffers() {
    bindBuffers();

    setDrawableBuffer(vertexBuffer,0);
    setDrawableBuffer(vertexColorBuffer,1);
    setDrawableBuffer(normalBuffer,2);
}

void oglu::RegisteredDrawable::drawInstances(const glm::mat4& MVP, const glm::vec3& light)
{
    bindProgram();
    bindBuffers();

    setInstancesDataArray(instancePositions, 3);
    setInstancesDataArray(instanceScales, 4);
    setInstancesDataArray(instanceRotations, 5);
    setInstancesDataArray(instanceColors, 6);

    glUniformMatrix4fv(0, 1, GL_FALSE, &MVP[0][0]);
    glUniform3f(1, light[0], light[1], light[2]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertexBuffer.size(), instances.size()); 
}