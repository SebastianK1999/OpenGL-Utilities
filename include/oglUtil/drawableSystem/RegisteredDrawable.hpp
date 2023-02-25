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


#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "oglUtil/drawableSystem/DrawableInstancePointers.hpp"
#include "oglUtil/drawableSystem/DrawableBase.hpp"

namespace oglu
{
    class RegisteredDrawable : public oglu::DrawableBase
    {
    public:
        std::shared_ptr<oglu::DrawableInstancePointers> addInstance();
        void reserve(const unsigned int numberOfNewInstances);
        void drawInstances(const glm::mat4& MVP, const glm::vec3& light);
        RegisteredDrawable
        (
            const std::string& key,
            const std::vector<glm::vec3> vertexBuffer,
            const std::vector<glm::vec4> vertexColorBuffer,
            const std::vector<glm::vec3> normalBuffer,
            const std::string& vertexShader,
            const std::string& fragmentShader
        );

    private:
        const std::string key;
        const std::vector<glm::vec3> vertexBuffer;
        const std::vector<glm::vec4> vertexColorBuffer;
        const std::vector<glm::vec3> normalBuffer;
        const std::string vertexShaderCode;
        const std::string fragmentShaderCode;
        GLuint shader;
        GLuint texture;
        std::vector<std::shared_ptr <oglu::DrawableInstancePointers>> instances;
        std::vector<glm::vec3> instancePositions;
        std::vector<glm::vec3> instanceScales;
        std::vector<glm::vec3> instanceRotations;
        std::vector<glm::vec4> instanceColors;
        void setShaders();
        void setBuffers();
    };
}