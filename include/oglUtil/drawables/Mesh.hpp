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

#include <memory>
#include <vector>
#include <tuple>
#include <filesystem>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "oglUtil/drawableSystem/DrawableInstance.hpp"
#include "oglUtil/drawableSystem/DrawableInstancePointers.hpp"
#include "oglUtil/drawableSystem/RegisteredDrawable.hpp"

namespace oglu
{
    class Mesh : public oglu::DrawableInstance
    {
    public:
        static std::string modelFileName;
        static std::string textureFileName;
        static std::shared_ptr<oglu::RegisteredDrawable> registerMesh(const std::filesystem::path& meshDirectory);
        static oglu::RegisteredDrawable& getRegistry(const std::filesystem::path& meshDirectory);
        static oglu::RegisteredDrawable& getRegistry(const std::string& key);
        static void drawInstances(const std::filesystem::path& meshDirectory, const glm::mat4& MVP, const glm::vec3& light);
        static void drawInstances(const std::string& key, const glm::mat4& MVP, const glm::vec3& light);
        
        oglu::RegisteredDrawable& getRegistry() const noexcept override;
        Mesh(const std::filesystem::path& meshDirectory);
        Mesh(oglu::RegisteredDrawable& meshTypeRegistry);
        Mesh(const Mesh& other)             = default;
        Mesh(Mesh&& other)                  = default;
        Mesh& operator=(const Mesh& other)  = default;
        Mesh& operator=(Mesh&& other)       = default;
        virtual ~Mesh()                     = default;    

    private:
        static const std::string createKey(const std::filesystem::path& meshDirectory) noexcept;
        static const bool loadObject
        (
            const std::filesystem::path& meshDirectory,
            std::vector<glm::vec3>& vertexBuffer,
            std::vector<glm::vec3>& normalBuffer,
            std::vector<glm::vec2>& uvBuffer
        );
        static const bool loadBMP
        (
            const std::filesystem::path& meshDirectory,
            std::vector<unsigned char>& textureData,
            unsigned int& width,
            unsigned int& height
        );
        std::shared_ptr<oglu::RegisteredDrawable> registryPointer;

    };
}
