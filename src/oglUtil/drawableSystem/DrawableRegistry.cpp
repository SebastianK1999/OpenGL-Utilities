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


#include <iostream>
#include <utility>

#include "oglUtil/drawableSystem/DrawableRegistry.hpp"

std::map<std::string, std::shared_ptr<oglu::RegisteredDrawable>> oglu::DrawableRegistry::registeredDrawables;

bool oglu::DrawableRegistry::hasRegistry(const std::string& key)
{
    return registeredDrawables.find(key) != registeredDrawables.end();
}

std::shared_ptr<oglu::RegisteredDrawable> oglu::DrawableRegistry::getRegistry(const std::string& key)
{
    return registeredDrawables.find(key)->second;
}

std::shared_ptr<oglu::RegisteredDrawable> oglu::DrawableRegistry::registerDrawable
(
    const std::string& key,
    const std::vector<glm::vec3> vertexBuffer,
    const std::vector<glm::vec4> vertexColorBuffer,
    const std::vector<glm::vec3> normalBuffer,
    const std::vector<glm::vec2> uvBuffer,
    const std::vector<unsigned char> textureData,
    const unsigned int textureWidth,
    const unsigned int textureHeight,
    const oglu::VertexShader& vertexShaderCode,
    const oglu::FragmentShader& fragmentShaderCode
)
{
    if(!hasRegistry(key)){
        registeredDrawables.insert(std::pair<std::string, std::shared_ptr<oglu::RegisteredDrawable>>
        (
        key,
        new oglu::RegisteredDrawable(
            key,
            vertexBuffer,
            vertexColorBuffer,
            normalBuffer,
            uvBuffer,
            textureData,
            textureWidth,
            textureHeight,
            vertexShaderCode,
            fragmentShaderCode
        )));
    }
    return getRegistry(key);
}

void oglu::DrawableRegistry::drawInstances(const std::string& key, const glm::mat4& MVP, const glm::vec3& light)
{
    getRegistry(key)->drawInstances(MVP, light);
}

void oglu::DrawableRegistry::reinitialize()
{
    for (auto drawableIterator = registeredDrawables.begin(); drawableIterator != registeredDrawables.end(); drawableIterator = std::next(drawableIterator))
    {
        drawableIterator->second->reinitialize();
    }
}