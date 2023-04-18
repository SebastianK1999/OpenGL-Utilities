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

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "oglUtil/drawableSystem/RegisteredDrawable.hpp"
#include "oglUtil/drawableSystem/DrawableInstancePointers.hpp"

namespace oglu
{
    class DrawableInstance
    {
    public:
        glm::vec3& getPosition() const noexcept;
        glm::vec3& getScale() const noexcept;
        glm::vec3& getRotation() const noexcept;
        glm::vec4& getColor() const noexcept;
        // virtual oglu::RegisteredDrawable getInstance() const noexcept   = 0;
        DrawableInstance();
        DrawableInstance(const DrawableInstance& other);
        DrawableInstance(DrawableInstance&& other);
        DrawableInstance& operator=(const DrawableInstance& other);
        DrawableInstance& operator=(DrawableInstance&& other);
        virtual ~DrawableInstance();
        virtual oglu::RegisteredDrawable& getRegistry() const noexcept  = 0;
    
    protected:
        std::shared_ptr<oglu::RegisteredDrawable> registryPointer;
        std::shared_ptr<oglu::DrawableInstancePointers> instancePointer; // TODO vector of drawInstPrt inside of registeredDrawable, so you dont need to update all pointes, only the instancePointers (the smart pointer)

    };
}