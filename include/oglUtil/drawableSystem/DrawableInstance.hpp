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
        glm::vec3& getPosition();
        glm::vec3& getScale();
        glm::vec3& getRotation();
        glm::vec4& getColor();
        // virtual oglu::RegisteredDrawable getInstance() const noexcept   = 0;
        virtual oglu::RegisteredDrawable& getRegistry() const noexcept  = 0;
        DrawableInstance()                                              = default;
        DrawableInstance(const DrawableInstance& other)                 = default;
        DrawableInstance(DrawableInstance&& other)                      = default;
        DrawableInstance& operator=(const DrawableInstance& other)      = default;
        DrawableInstance& operator=(DrawableInstance&& other)           = default;
        virtual ~DrawableInstance()                                     = default;
    
    protected:
        std::shared_ptr<oglu::DrawableInstancePointers> instancePointer;

    };
}