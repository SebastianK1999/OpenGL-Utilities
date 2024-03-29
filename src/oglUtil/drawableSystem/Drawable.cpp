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


#include "oglUtil/drawableSystem/Drawable.hpp"
#include <utility>
#include <iostream>

oglu::Drawable::Drawable(const Drawable& other) noexcept 
: DrawableBase(other)
, position(other.position)
, scale(other.scale)
, rotation(other.rotation)
{}

oglu::Drawable::Drawable(Drawable&& other) noexcept 
: DrawableBase(std::move(other))            // have no idea why it calls default constructor
, position(std::move(other.position))
, scale(std::move(other.scale))
, rotation(std::move(other.rotation))
{}

oglu::Drawable& oglu::Drawable::operator=(const Drawable& other) noexcept {
    position = other.position;
    scale = other.scale;
    rotation = other.rotation;
    return *this;
}

oglu::Drawable& oglu::Drawable::operator=(Drawable&& other) noexcept {
    if (this != &other) {
        position = std::move(other.position);
        scale = std::move(other.scale);
        rotation = std::move(other.rotation);
    }
    return *this;
}

oglu::Drawable::~Drawable() {}

oglu::Drawable::Drawable()
: DrawableBase()
, position(0,0,0,0)
, scale(1,1,1,1)
, rotation(0,0,0,0)
{}