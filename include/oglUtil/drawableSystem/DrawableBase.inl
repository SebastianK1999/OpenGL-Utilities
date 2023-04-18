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

/*
*   Inline file for DrawableBase.hpp
*/

#pragma once 

template <glm::length_t L,typename T, glm::qualifier Q>
void oglu::DrawableBase::setInstancesDataArray(const std::vector<glm::vec<L,T,Q>>& instancesDataArray, const unsigned int layoutAttribute)
{
    const unsigned int instancesCount = (instancesDataArray.size() < oglu::DrawableBase::limitDrawables ? instancesDataArray.size() : oglu::DrawableBase::limitDrawables);
    GLuint arrayId;
    glGenBuffers(1, &arrayId);
    glBindBuffer(GL_ARRAY_BUFFER, arrayId);
    glBufferData
    (
        GL_ARRAY_BUFFER,
        sizeof(T) * L * instancesCount,
        instancesDataArray.data(),
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(layoutAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, arrayId);
    glVertexAttribPointer
    (
        layoutAttribute,
        L,
        GL_FLOAT,
        GL_FALSE,
        sizeof(T) * L,
        (void*)0
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(layoutAttribute,1);
}

template <glm::length_t L,typename T, glm::qualifier Q>
void oglu::DrawableBase::setDrawableBuffer(const std::vector<glm::vec<L,T,Q>>& drawableBuffer, const unsigned int layoutAttribute)
{
    GLuint bufferId;    
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData
    (
        GL_ARRAY_BUFFER,
        sizeof(T) * L * drawableBuffer.size(),
        drawableBuffer.data(),
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(layoutAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glVertexAttribPointer
    (
        layoutAttribute,                    // attribute, must match the layout in the shader.
        L,  // size
        GL_FLOAT,                           // type
        GL_FALSE,                           // normalized?
        0,                                  // stride
        (void*)0                            // array buffer offset
    );
}