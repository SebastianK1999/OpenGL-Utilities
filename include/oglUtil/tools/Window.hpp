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
#include <array>
#include <oglUtil/tools/KeyTracker.hpp>

namespace oglu 
{
    class Window
    {
    public:
        KeyTracker keyTracker;

        Window(const Window& other);
        Window(Window&& other);
        Window& operator=(const Window& other);
        Window& operator=(Window&& other);
        virtual ~Window();
        Window(const int width, const int height, const std::string& name, const bool fullscreen=false, const bool fullscreenLastMonitor=false, const bool makeContextCurrent=true);
        Window();

        bool isFullScreen() const ;
        void setFullScreen(const bool fullscreen, const bool fullscreenLastMonitor = false);
        void switchFullScreen(const bool fullscreenLastMonitor = false);
        void destroy();
        GLFWwindow* getGlfwWindowPtr() const;
        
        virtual void mainLoop();
        //.virtual void mainLoopThread();
        virtual void renderWindow();
        virtual void mainFunction();
        virtual void drawFunction();
        virtual void resize(const int width, const int height);
        virtual void afterResize(const int width, const int height);
		virtual void keyCallback(const int key, const int scancode, const int action, const int mods );
		virtual void mousePosCallback(const double x, const double y );
		virtual void mouseButtonCallback(const int button, const int action,const int mods );
		virtual void scrollCallback(const double x, const double y );

    private:
        GLFWwindow* glfwWindowPtr;
        std::array<int, 2> windowedModePosition;
        std::array<int, 2> windowedModeSize;

//////////////////////////////////////////////////////////////////////////////////////
/*
    public:    
        void  GetCursorNDC(float *x, float *y);            // mouse position VpOne


        void WaitForFixedFPS(float frame_time=1./60.); // Use before glfwSwapBuffers()

    private:

        int vtx, vty, vwd, vht; // GtCursorNDC() and ViewportOne()

        // vers=33 for GLSL 3.3, fullscr=0 for window or 1/2 for FullScr 1/2 monitor

        double prev_time=0.0; // for fixed FPS
*/
////////////////////////////////////////////////////////////////////////////////////////////
    };
}