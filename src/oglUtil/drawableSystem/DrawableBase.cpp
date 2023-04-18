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


#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include "oglUtil/drawableSystem/DrawableBase.hpp"

unsigned int oglu::DrawableBase::limitDrawables = -1; 

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

void oglu::DrawableBase::compileShaders
(
   const char*const vs,
   const char*const fs,
   const char*const gs
)
{
    GLuint  v = glCreateShader(GL_VERTEX_SHADER);
    GLuint  f = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint  g = 0;
    if (gs) g = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(v, 1, &vs, NULL);   // Also read from file: next fun
    glShaderSource(f, 1, &fs, NULL);   // ...
    if (gs) glShaderSource(g, 1, &gs, NULL);   // ...

    shaderId = compileShaders(v,f,g);
    glUseProgram(programId);
}

void oglu::DrawableBase::bindTexture() const
{
   glBindTexture(GL_TEXTURE_2D, textureId);
}

void oglu::DrawableBase::bindProgram() const
{
   glUseProgram(programId);
}

void oglu::DrawableBase::bindBuffers() const
{
   glBindVertexArray(            vaoId);
   glBindBuffer(GL_ARRAY_BUFFER, vboId);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
}   

void oglu::DrawableBase::removeProgram() const
{
   glDeleteShader(programId);
}

GLuint oglu::DrawableBase::compileShaders(const GLuint v, const GLuint f, const GLuint g) {
   GLint Result = GL_FALSE; // TODO - bool or remove
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

void oglu::DrawableBase::compileTexture
(
   const std::vector<unsigned char>& textureData,
   const unsigned int width,
   const unsigned int height
)
{
	glGenTextures(1, &textureId);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureId);
   glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
	// Give the image to OpenGL
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D
   (
      GL_TEXTURE_2D,
      0,
      GL_RGB,
      width,
      height,
      0,
      GL_BGR,
      GL_UNSIGNED_BYTE,
      textureData.data()
   );
   constexpr bool poorFilter = false; // poor means sharp pixels
   if constexpr (poorFilter)
   {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
   }
   else
   {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   }
	glGenerateMipmap(GL_TEXTURE_2D);
}
        