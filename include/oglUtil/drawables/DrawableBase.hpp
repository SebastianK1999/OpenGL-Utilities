/*
* MIT License
* 
* Copyright (c) 2022 Sebastian Kwaśniak
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

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace oglu 
{
    class DrawableBase
    {
    public:
        DrawableBase(const DrawableBase& other) noexcept;
        DrawableBase(DrawableBase&& other) noexcept;
        DrawableBase& operator=(const DrawableBase& other) noexcept;
        DrawableBase& operator=(DrawableBase&& other) noexcept;
        virtual ~DrawableBase();
        DrawableBase(const GLuint pid=0);
        int compileShaders(const char*const vs, const char*const fs, const char*const gs=NULL);
        int compileShadersFromFile(const char*const vs, const char*const fs, const char*const gs=NULL);
        void bindVAO() const;
        void bindBuffers() const;
        void bindProgram() const;
        GLuint getProgramId() const;

        private:
        GLuint vaoId;       // Vertex Array Ocjects
        GLuint vboId;       // Vertex Buffer Objects
        GLuint eboId;       // Element Buffer Objects
        GLuint programId;
        GLint compileLink(const GLuint v, const char*const which, int prog=0) const;
        int compileShaders(const GLuint v, const GLuint f, const GLuint g=0);
        void getShaderSource(const GLuint sId, const char*const  file) const;
    };
}