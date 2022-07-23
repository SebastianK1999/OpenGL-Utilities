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


#include "oglUtil/tools/Window.hpp"
#include "oglUtil/tools/OgluFunctions.hpp"

oglu::Window::Window(const Window& other):
    keyTracker(other.keyTracker),
    glfwWindowPtr(other.glfwWindowPtr),
    windowedModeSize(other.windowedModeSize),
    windowedModePosition(other.windowedModePosition)
{}

oglu::Window::Window(Window&& other):
    keyTracker(std::move(other.keyTracker)),
    glfwWindowPtr(std::move(other.glfwWindowPtr)),
    windowedModeSize(std::move(other.windowedModeSize)),
    windowedModePosition(std::move(other.windowedModePosition))
{}

oglu::Window& oglu::Window::operator=(const Window& other){
    keyTracker = other.keyTracker;
    // glfwWindowPtr = other.glfwWindowPtr; // copy data, not identity
    windowedModeSize = other.windowedModeSize;
    windowedModePosition = other.windowedModePosition;
    return *this;
}

oglu::Window& oglu::Window::operator=(Window&& other){
    if (this != &other) {
        keyTracker = std::move(other.keyTracker);
        glfwWindowPtr = std::move(other.glfwWindowPtr);
        windowedModeSize = std::move(other.windowedModeSize);
        windowedModePosition = std::move(other.windowedModePosition);
    }
    return *this;
}

oglu::Window::~Window(){
   glfwDestroyWindow(glfwWindowPtr);
}

oglu::Window::Window(const int width, const int height, const std::string& name, const bool fullscreen, const bool fullscreenLastMonitor, const bool makeContextCurrent){
    if (fullscreen) {
        int monitorCount;
        GLFWmonitor** monitors= glfwGetMonitors(&monitorCount);
        GLFWmonitor*& monitor = monitors[(fullscreenLastMonitor ? monitorCount-1 : 0)];
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwWindowPtr = glfwCreateWindow(mode->width, mode->height, name.c_str(), monitor, nullptr);
    } 
    else {
        glfwWindowPtr = glfwCreateWindow( width, height, name.c_str(), NULL, NULL);
    }
    if(makeContextCurrent){
        glfwMakeContextCurrent(glfwWindowPtr); 
    }

    glfwSetWindowUserPointer(glfwWindowPtr, this );
    glfwSetWindowSizeCallback(glfwWindowPtr, oglu::callbackResize );
    glfwSetKeyCallback(glfwWindowPtr, oglu::callbackKey );
    glfwSetCursorPosCallback(glfwWindowPtr, oglu::callbackMousePos );
    glfwSetMouseButtonCallback(glfwWindowPtr, oglu::callbackMouseButton );
    glfwSetScrollCallback(glfwWindowPtr, oglu::callbackScroll );

    glfwGetWindowPos(  glfwWindowPtr, &windowedModePosition[0],  &windowedModePosition[1] );
    // Resize() begining:
    // Ensure we can capture the escape key being pressed below
    //glfwSetInputMode(glfwWindowPtr, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSwapInterval(1);  // 60 fps ?
    glEnable(GL_MULTISAMPLE);

    glfwSetInputMode(getGLFWwindowPtr(), GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.4f, 0.0f, 0.0f, 0.0f);
}

oglu::Window::Window()
: Window(800, 600, "(Untitled)")
{

}

bool oglu::Window::isFullScreen() const {
    return glfwGetWindowMonitor( glfwWindowPtr ) != nullptr;
}

void oglu::Window::setFullScreen(const bool fullscreen, const bool fullscreenLastMonitor){
    if ( isFullScreen() == fullscreen )
        return;
    if ( fullscreen ) {
        glfwGetWindowPos(glfwWindowPtr, &(windowedModePosition[0]), &(windowedModePosition[1]));
        glfwGetWindowSize(glfwWindowPtr, &(windowedModeSize[0]), &(windowedModeSize[1]));
        int monitorCount;
        GLFWmonitor** monitors= glfwGetMonitors(&monitorCount);
        GLFWmonitor*& monitor = monitors[(fullscreenLastMonitor ? monitorCount-1 : 0)];
        const GLFWvidmode * mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(glfwWindowPtr, monitor, 0, 0, mode->width, mode->height, 0);
        glViewport(0,0, mode->width, mode->height);
        glScissor(0,0, mode->width, mode->height);

    } else {
        glfwSetWindowMonitor(glfwWindowPtr, nullptr,  (windowedModePosition[0]), (windowedModePosition[1]), (windowedModeSize[0]), (windowedModeSize[1]), 0 );
        glfwShowWindow(glfwWindowPtr);
        glViewport(0,0, (windowedModeSize[0]), (windowedModeSize[1]));
        glScissor(0,0, (windowedModeSize[0]), (windowedModeSize[1]));

    }
    // int vwd;
    // int vht;
    // glfwGetFramebufferSize(glfwWindowPtr, &vwd, &vht);
    // //glfwGetWindowSize(glfwWindowPtr, &vwd, &vht );
    // glViewport(0,0,vwd,vht);
    // glScissor(0,0,vwd,vht);
}

void oglu::Window::switchFullScreen(const bool fullscreenLastMonitor){
    setFullScreen(! isFullScreen(), fullscreenLastMonitor);
}

GLFWwindow* oglu::Window::getGLFWwindowPtr() const {
    return glfwWindowPtr;
}

void oglu::Window::mainLoop(){
    do {
        renderWindow();
    } while( glfwWindowShouldClose(getGLFWwindowPtr()) == 0 && glfwWindowPtr != nullptr);
}

//.void oglu::Window::mainLoopThread();

void oglu::Window::renderWindow(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    printErrorsDbg("glClear");
    keyTracker.update(glfwWindowPtr);
    mainFunction();
    printErrorsDbg("mainFunction");
    drawFunction();
    printErrorsDbg("drawFunction");
    glfwSwapBuffers(getGLFWwindowPtr()); // Swap buffers
    glfwPollEvents();
    printErrorsDbg("glfwSwapBuffers or glfwPollEvents");
}

void oglu::Window::mainFunction(){

}

void oglu::Window::drawFunction(){

}

void oglu::Window::resize(const int width, const int height){
    int vwd;
    int vht;
    glfwGetFramebufferSize(glfwWindowPtr, &vwd, &vht);
    //glfwGetWindowSize(glfwWindowPtr, &vwd, &vht );
    glViewport(0,0,vwd,vht);
    //glScissor(0,0,vwd,vht);
}

void oglu::Window::keyCallback(const int key, const int scancode, const int  action, const int mods ){

}

void oglu::Window::mousePosCallback(const double x, const double y ){

}

void oglu::Window::mouseButtonCallback( const int button, const int action, const int mods ){

}

void oglu::Window::scrollCallback( const double x, const double y ){

}