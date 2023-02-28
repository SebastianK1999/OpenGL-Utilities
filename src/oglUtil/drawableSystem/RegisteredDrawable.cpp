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
#include "oglUtil/tools/FileReader.hpp"
#include "oglUtil/drawableSystem/ShaderLayoutEnums.hpp"

#include "oglUtil/drawableSystem/RegisteredDrawable.hpp"

std::string oglu::RegisteredDrawable::vertexShaderFileName = "shader.vs";

std::string oglu::RegisteredDrawable::fragmentShaderFileName = "shader.fs";

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

oglu::RegisteredDrawable::RegisteredDrawable
(
    const std::string& arg_key,
    const std::vector<glm::vec3> arg_vertexBuffer,
    const std::vector<glm::vec4> arg_vertexColorBuffer,
    const std::vector<glm::vec3> arg_normalBuffer,
    const std::vector<glm::vec2> arg_uvBuffer,
    const std::vector<unsigned char> arg_textureData,
    const unsigned int arg_textureWidth,
    const unsigned int arg_textureHeight,
    const oglu::VertexShader& arg_vertexShaderCode,
    const oglu::FragmentShader& arg_fragmentShaderCode
)
: oglu::DrawableBase()
, key(arg_key)
, vertexBuffer(arg_vertexBuffer)
, vertexColorBuffer(arg_vertexColorBuffer)
, normalBuffer(arg_normalBuffer)
, uvBuffer(arg_uvBuffer)
, textureData(arg_textureData)
, textureWidth(arg_textureWidth)
, textureHeight(arg_textureHeight)
, vertexShaderCode(arg_vertexShaderCode)
, fragmentShaderCode(arg_fragmentShaderCode)
{
    setTexture();
    setShaders();
    setBuffers();
}

void oglu::RegisteredDrawable::setTexture()
{
    compileTexture(textureData, textureWidth, textureHeight);
}

void oglu::RegisteredDrawable::setShaders()
{
    compileShaders(vertexShaderCode.c_str(), fragmentShaderCode.c_str());
}

void oglu::RegisteredDrawable::loadShaders(const std::filesystem::path& shadersDirectory)
{
    oglu::VertexShader vertexShaderCode = oglu::FileReader::readFileContent(shadersDirectory / oglu::RegisteredDrawable::vertexShaderFileName);
    oglu::FragmentShader fragmentShaderCode = oglu::FileReader::readFileContent(shadersDirectory / oglu::RegisteredDrawable::fragmentShaderFileName);
    loadShaders
    (
        oglu::ShaderCollection::vertexShaderLayouts + oglu::ShaderCollection::vertexShaderFunctions + vertexShaderCode,
        oglu::ShaderCollection::fragmentShaderLayouts + fragmentShaderCode
    );
}

void oglu::RegisteredDrawable::loadShaders
(
    const oglu::VertexShader& vertexShaderCode,
    const oglu::FragmentShader& fragmentShaderCode
)
{
    removeProgram();
    compileShaders(vertexShaderCode.c_str(), fragmentShaderCode.c_str());
}

void oglu::RegisteredDrawable::setBuffers() {
    bindBuffers();

    setDrawableBuffer(vertexBuffer, oglu::EShaderBufferLayout::vertex);
    
    if (vertexBuffer.size() == vertexColorBuffer.size())
    {
        setDrawableBuffer(vertexColorBuffer,oglu::EShaderBufferLayout::vertexColor);
    }
    if (vertexBuffer.size() == normalBuffer.size())
    {
        setDrawableBuffer(normalBuffer, oglu::EShaderBufferLayout::normal);
    }
    if (vertexBuffer.size() == uvBuffer.size())
    {
        setDrawableBuffer(uvBuffer, oglu::EShaderBufferLayout::uv);
    }  
}

void oglu::RegisteredDrawable::drawInstances(const glm::mat4& MVP, const glm::vec3& light)
{
    bindTexture();
    bindProgram();
    bindBuffers();

    setInstancesDataArray(instancePositions,oglu::EShaderBufferLayout::position);
    setInstancesDataArray(instanceScales,   oglu::EShaderBufferLayout::scale);
    setInstancesDataArray(instanceRotations,oglu::EShaderBufferLayout::rotation);
    setInstancesDataArray(instanceColors,   oglu::EShaderBufferLayout::color);

    glUniformMatrix4fv(0, 1, GL_FALSE, &MVP[0][0]);
    glUniform3f(1, light[0], light[1], light[2]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertexBuffer.size(), instances.size()); 
}
