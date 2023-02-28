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

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "oglUtil/drawableSystem/DrawableInstance.hpp"
#include "oglUtil/drawableSystem/DrawableInstancePointers.hpp"
#include "oglUtil/drawableSystem/RegisteredDrawable.hpp"

namespace oglu
{
    class Sphere : public oglu::DrawableInstance
    {
    public:
        static unsigned int levelOfDetail;
        static constexpr char key[] = "_oglu::Sphere";
        static const std::vector<glm::vec3> generateMesh();
        static const std::vector<glm::vec3> generateNormals(const std::vector<glm::vec3>& vertexBuffer);
        static void drawInstances(const glm::mat4& MVP, const glm::vec3& light);
        // static draw(const glm::mat4& MVP, const glm::vec3& light);
        oglu::RegisteredDrawable& getRegistry() const noexcept override;
        Sphere();
        Sphere(const Sphere& other)             = default;
        Sphere(Sphere&& other)                  = default;
        Sphere& operator=(const Sphere& other)  = default;
        Sphere& operator=(Sphere&& other)       = default;
        virtual ~Sphere()                       = default;

    private:
        static std::shared_ptr<oglu::RegisteredDrawable> registryPointer;

    };
}
