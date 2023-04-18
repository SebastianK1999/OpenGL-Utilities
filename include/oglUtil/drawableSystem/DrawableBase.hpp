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


/* WITH HELP OF */
// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGLDrawable
//===========================================================================


#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace oglu 
{
    class DrawableBase
    {
    public:
        static unsigned int limitDrawables; 
        DrawableBase(const DrawableBase& other) noexcept;
        DrawableBase(DrawableBase&& other) noexcept;
        DrawableBase& operator=(const DrawableBase& other) noexcept;
        DrawableBase& operator=(DrawableBase&& other) noexcept;
        virtual ~DrawableBase();
        DrawableBase(const GLuint pid=0);

    protected:
        GLuint vaoId;       // Vertex Array Objects
        GLuint vboId;       // Vertex Buffer Objects
        GLuint eboId;       // Element Buffer Objects
        GLuint programId;
        GLuint shaderId;
        GLuint textureId;
        void compileShaders
        (
            const char*const vs,
            const char*const fs,
            const char*const gs=NULL
        );
        void compileTexture 
        (
            const std::vector<unsigned char>& textureData,
            const unsigned int width,
            const unsigned int height
        );
        void bindTexture() const;
        void bindProgram() const;
        void bindBuffers() const;
        void removeProgram() const;
        template <glm::length_t L,typename T, glm::qualifier Q>
        void setInstancesDataArray(const std::vector<glm::vec<L,T,Q>>& instancesDataArray, const unsigned int layoutAttribute);
        template <glm::length_t L,typename T, glm::qualifier Q>
        void setDrawableBuffer(const std::vector<glm::vec<L,T,Q>>& drawableBuffer, const unsigned int layoutAttribute);
        GLuint getProgramId() const;
    
    private:
        GLuint compileShaders
        (
            const GLuint v,
            const GLuint f,
            const GLuint g=0
        );
        GLint compileLink
        (
            const GLuint v,
            const char*const which,
            int prog=0
        ) const;

    };
}

#include "oglUtil/drawableSystem/DrawableBase.inl"