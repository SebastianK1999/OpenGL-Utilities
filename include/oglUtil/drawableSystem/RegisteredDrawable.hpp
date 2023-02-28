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
#include <filesystem>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "oglUtil/drawableSystem/ShaderCollection.hpp"
#include "oglUtil/drawableSystem/DrawableInstancePointers.hpp"
#include "oglUtil/drawableSystem/DrawableBase.hpp"

namespace oglu
{
    class RegisteredDrawable : public oglu::DrawableBase
    {
    public:
        static std::string vertexShaderFileName;
        static std::string fragmentShaderFileName;

        const std::string key;
        std::shared_ptr<oglu::DrawableInstancePointers> addInstance();
        void reserve(const unsigned int numberOfNewInstances);
        void drawInstances(const glm::mat4& MVP, const glm::vec3& light);
        void loadShaders(const std::filesystem::path& shadersDirectory);
        void loadShaders
        (
            const oglu::VertexShader& vertexShader,
            const oglu::FragmentShader& FragmentShader
        );
        RegisteredDrawable
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
        );

    private:
        const std::vector<glm::vec3> vertexBuffer;
        const std::vector<glm::vec4> vertexColorBuffer;
        const std::vector<glm::vec3> normalBuffer;
        const std::vector<glm::vec2> uvBuffer;
        const std::vector<unsigned char> textureData;
        const unsigned int textureWidth;
        const unsigned int textureHeight;
        const std::string vertexShaderCode;
        const std::string fragmentShaderCode;
        GLuint shader;
        GLuint texture;
        std::vector<std::shared_ptr <oglu::DrawableInstancePointers>> instances;
        std::vector<glm::vec3> instancePositions;
        std::vector<glm::vec3> instanceScales;
        std::vector<glm::vec3> instanceRotations;
        std::vector<glm::vec4> instanceColors;
        void setTexture();
        void setShaders();
        void setBuffers();
    };
}