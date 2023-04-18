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

#include "oglUtil/drawableSystem/DrawableInstance.hpp"

glm::vec3& oglu::DrawableInstance::getPosition() const noexcept
{
    return *(instancePointer->positionPointer);
}

glm::vec3& oglu::DrawableInstance::getScale() const noexcept
{
    return *(instancePointer->scalePointer);
}

glm::vec3& oglu::DrawableInstance::getRotation() const noexcept
{
    return *(instancePointer->rotationPointer);
}

glm::vec4& oglu::DrawableInstance::getColor() const noexcept
{
    return *(instancePointer->colorPointer);
}

oglu::DrawableInstance::DrawableInstance()
    : registryPointer(nullptr)
    , instancePointer()
{
}

oglu::DrawableInstance::DrawableInstance(const DrawableInstance& other)
    : registryPointer(other.registryPointer)
    , instancePointer(other.registryPointer->addInstance())
{
        instancePointer->copyValues(*(other.instancePointer));
}

oglu::DrawableInstance::DrawableInstance(DrawableInstance&& other)
    : registryPointer(std::move(other.registryPointer))
    , instancePointer(std::move(other.instancePointer))
{
}

oglu::DrawableInstance& oglu::DrawableInstance::operator=(const DrawableInstance& other)
{
    if(this != &other){
        registryPointer = other.registryPointer;
        instancePointer = other.registryPointer->addInstance();
        instancePointer->copyValues(*(other.instancePointer));
    }
    return *this;
}

oglu::DrawableInstance& oglu::DrawableInstance::operator=(DrawableInstance&& other)
{
    registryPointer = std::move(other.registryPointer);
    instancePointer = std::move(other.instancePointer);
    return *this;
}

oglu::DrawableInstance::~DrawableInstance()
{
    if(instancePointer.use_count() == 2)  // 2 located in this and registryPointer.instances
    {
        registryPointer->deleteInstance(instancePointer);
    }
}