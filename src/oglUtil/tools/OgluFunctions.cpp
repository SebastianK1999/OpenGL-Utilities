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


#include <iostream>
#include <iomanip>
#include <GL/glew.h>
#include "oglUtil/tools/OgluFunctions.hpp"
#include "oglUtil/tools/Window.hpp"

bool oglu::setupOglu(){
	// TODO
	// check log file ? create : pass
	// write date time to it

	return true;
}

bool oglu::setupGlfw(){
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return true;
}

bool oglu::setupGlew(const bool experimental){
	glewExperimental = experimental; 
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return false;
	}
    return true;
}

void oglu::printErrors(const std::string& comment){
   GLenum errorCode;
   while ( (errorCode=glGetError()) ){
		std::cout << "OpenGL ERROR: 0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << errorCode << " : " << comment << ";" << "\n";
   }
}

void oglu::printErrorsDbg(const std::string& comment){
#ifdef DEBUG
   GLenum errorCode;
   while ( (errorCode=glGetError()) ){
		std::cout << "(Dbg) OpenGL ERROR: 0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << errorCode << " : " << comment << ";" << "\n";
   }
#endif
}

void oglu::logErrors(const std::string& file_path, const std::string& comment){
// TODO
}

void oglu::logErrorsDbg(const std::string& file_path, const std::string& comment){
#ifdef DEBUG
// TODO
#endif
}

void oglu::callbackResize(GLFWwindow* window, int width, int height) {
   	void* userWindowPtr = glfwGetWindowUserPointer( window );
   	oglu::Window *ogluWindowPtr = static_cast<oglu::Window*>( userWindowPtr );
   	if ( ogluWindowPtr ){
		ogluWindowPtr->resize(width, height);
   	}
}

void oglu::callbackKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
   	void* userWindowPtr = glfwGetWindowUserPointer( window );
   	oglu::Window *ogluWindowPtr = static_cast<oglu::Window*>( userWindowPtr );
   	if ( ogluWindowPtr ){
		ogluWindowPtr->keyCallback( key, scancode, action, mods );
   	}
}

void oglu::callbackMousePos(GLFWwindow* window, double x, double y) {
   	void* userWindowPtr = glfwGetWindowUserPointer( window );
   	oglu::Window *ogluWindowPtr = static_cast<oglu::Window*>( userWindowPtr );
   	if ( ogluWindowPtr ){
		ogluWindowPtr->mousePosCallback( x, y );
   	}
}

void oglu::callbackMouseButton(GLFWwindow* window, int button, int action, int mods) {
   	void* userWindowPtr = glfwGetWindowUserPointer( window );
   	oglu::Window *ogluWindowPtr = static_cast<oglu::Window*>( userWindowPtr );
   	if ( ogluWindowPtr ){
		ogluWindowPtr->mouseButtonCallback( button, action, mods );
   	}
}

void oglu::callbackScroll(GLFWwindow* window, double x, double y) {
   	void* userWindowPtr = glfwGetWindowUserPointer( window );
   	oglu::Window *ogluWindowPtr = static_cast<oglu::Window*>( userWindowPtr );
   	if ( ogluWindowPtr ){
		ogluWindowPtr->scrollCallback( x, y );
   	}
}