/*
*   Inline file for DrawableBase.hpp
*/

#pragma once 

template <glm::length_t L,typename T, glm::qualifier Q>
void oglu::DrawableBase::setInstancesDataArray(const std::vector<glm::vec<L,T,Q>>& instancesDataArray, const unsigned int layoutAttribute)
{
    GLuint arrayId;
    glGenBuffers(1, &arrayId);
    glBindBuffer(GL_ARRAY_BUFFER, arrayId);
    glBufferData
    (
        GL_ARRAY_BUFFER,
        sizeof(T) * L * instancesDataArray.size(),
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