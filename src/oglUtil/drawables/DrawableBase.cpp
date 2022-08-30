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


#include "oglUtil/drawables/DrawableBase.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

oglu::DrawableBase::DrawableBase(const DrawableBase& other) noexcept
: vaoId(other.vaoId)
, vboId(other.vboId)
, eboId(other.eboId)
, programId(other.programId)
{
   glGenVertexArrays(1, &vaoId);
   // glGenBuffers(     1, &vboId);
   // glGenBuffers(     1, &eboId);
}

oglu::DrawableBase::DrawableBase(DrawableBase&& other) noexcept
: vaoId(std::move(other.vaoId))
, vboId(std::move(other.vboId))
, eboId(std::move(other.eboId))
, programId(std::move(other.programId))
{
   glGenVertexArrays(1, &vaoId);
   // glGenBuffers(     1, &vboId);
   // glGenBuffers(     1, &eboId);
}

oglu::DrawableBase& oglu::DrawableBase::operator=(const DrawableBase& other) noexcept {
    vaoId = other.vaoId;
    vboId = other.vboId;
    eboId = other.eboId;
    programId = other.programId;
    return *this;
}

oglu::DrawableBase& oglu::DrawableBase::operator=(DrawableBase&& other) noexcept {
    if (this != &other) {
        vaoId = std::move(other.vaoId);
        vboId = std::move(other.vboId);
        eboId = std::move(other.eboId);
        programId = std::move(other.programId);
    }
    return *this;
}

oglu::DrawableBase::~DrawableBase() {
   glDeleteBuffers(1, &vboId);
   glDeleteBuffers(1, &eboId);
   glDeleteVertexArrays(1, &vaoId);
   if (programId) {
      glDeleteProgram(programId);
   }
}

oglu::DrawableBase::DrawableBase(const GLuint _pId)
: vaoId(_pId)
, vboId(0)
, eboId(0)
, programId(0)
{
   glGenVertexArrays(1, &vaoId);
   // glGenBuffers(     1, &vboId);
   // glGenBuffers(     1, &eboId);
}

int oglu::DrawableBase::compileShaders(const char*const vs, const char*const fs, const char*const gs) {
    GLuint  v = glCreateShader(GL_VERTEX_SHADER);
    GLuint  f = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint  g = 0;
    if (gs) g = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(v, 1, &vs, NULL);   // Also read from file: next fun
    glShaderSource(f, 1, &fs, NULL);   // ...
    if (gs) glShaderSource(g, 1, &gs, NULL);   // ...

    int res = compileShaders(v,f,g);
    glUseProgram(getProgramId());
    return res;
}

int oglu::DrawableBase::compileShadersFromFile(const char*const vs, const char*const fs, const char*const gs) {
   GLuint  v = glCreateShader(GL_VERTEX_SHADER);
   GLuint  f = glCreateShader(GL_FRAGMENT_SHADER);
   GLuint  g = 0;
   if (gs) g = glCreateShader(GL_GEOMETRY_SHADER);
   getShaderSource(v, vs);
   getShaderSource(f, fs);
   if (gs) getShaderSource(g, gs);

   int res = compileShaders(v,f,g);
   glUseProgram(getProgramId());
   return res;
}

void oglu::DrawableBase::bindVAO() const {
   glBindVertexArray(vaoId);
}

void oglu::DrawableBase::bindBuffers() const {
   glBindVertexArray(            vaoId);
   glBindBuffer(GL_ARRAY_BUFFER, vboId);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
}
   
void oglu::DrawableBase::bindProgram() const {
   glUseProgram(programId);
}

GLuint oglu::DrawableBase:: getProgramId() const {
   return programId;
}

int oglu::DrawableBase::compileShaders(const GLuint v, const GLuint f, const GLuint g) {
    GLint Result = GL_FALSE;
    if (g) Result = compileLink(g, "GS");
    if ( (Result=compileLink(v, "VS")) )
        if ( compileLink(f, "FS") ) {
        programId = glCreateProgram();
        glAttachShader(programId,v);
        glAttachShader(programId,f);
        if (g) glAttachShader(programId,g);
        compileLink(programId, "Linking",  3);
        }
    glDeleteShader(v);
    glDeleteShader(f);
    if (g) glDeleteShader(g);
    return Result;
}

GLint oglu::DrawableBase::compileLink(const GLuint v, const char*const which, int prog) const {
   GLint Result = GL_FALSE;
   int InfoLogLength;
   if (prog) {
      glLinkProgram(v);
      glGetProgramiv(v, GL_LINK_STATUS, &Result);
      glGetProgramiv(v, GL_INFO_LOG_LENGTH, &InfoLogLength);
   } else {
      glCompileShader(v);
      glGetShaderiv(v, GL_COMPILE_STATUS, &Result);
      glGetShaderiv(v, GL_INFO_LOG_LENGTH, &InfoLogLength);
   }
   if ( InfoLogLength > 0 && !Result ) {
      std::vector<char> Message(InfoLogLength+1);
      if (prog)
         glGetProgramInfoLog(v,InfoLogLength,NULL,&Message[0]);
      else
         glGetShaderInfoLog(v,InfoLogLength,NULL,&Message[0]);
      printf("%s: %s\n", which, &Message[0]);
   }
   return Result;
}
void oglu::DrawableBase::getShaderSource(const GLuint sId, const char*const  file) const {
      std::string sCode;
      std::ifstream sStream(file, std::ios::in);
      if(sStream.is_open()) {
         std::string Line = "";
         while(getline(sStream, Line))
            sCode += "\n" + Line;
         sStream.close();
      } else {
         printf("Error opening file:  %s !\n", file);
         getchar();
         return;
      }
      char const * SourcePointer = sCode.c_str();
      glShaderSource(sId, 1, &SourcePointer , NULL);
}

   