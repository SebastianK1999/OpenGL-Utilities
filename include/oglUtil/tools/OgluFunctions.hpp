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


#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace oglu
{
    bool setupOglu();
    bool setupGlfw();
    bool setupGlew(const bool experimental = false);
    void printErrors(const std::string& comment);
    void printErrorsDbg(const std::string& comment);
    void logErrors(const std::string& file_path, const std::string& comment);
    void logErrorsDbg(const std::string& file_path, const std::string& comment);
    void callbackResize(GLFWwindow* window, int width, int height);
    void callbackKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    void callbackMouseButton(GLFWwindow* window, int button, int action, int mods);
    void callbackScroll(GLFWwindow* window, double xp, double yp);
    void callbackMousePos(GLFWwindow* window, double xp, double yp);
}