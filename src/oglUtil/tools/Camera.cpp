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
* SOFTWARE.*/


#include "oglUtil/tools/Camera.hpp"
#include <utility>
#include <glm/glm.hpp>

oglu::Camera::Camera(const Camera& other)
: window(other.window)
, radius(other.radius)
, fieldOfView(other.fieldOfView)
, aspect(other.aspect)
, rotationSpeed(other.rotationSpeed)
, minVerticalRotation(other.minVerticalRotation)
, maxVerticalRotation(other.maxVerticalRotation)
, position(other.position) 
, rotation(other.rotation)
, direction(other.direction)
, up(other.up)
, right(other.right)
, viewMatrix(other.viewMatrix)
, projectionMatrix(other.projectionMatrix)
, basicModelMatrix(other.basicModelMatrix)
, MVP(other.MVP)
{}

oglu::Camera::Camera(Camera&& other)
: window(std::move(other.window))
, radius(std::move(other.radius))
, fieldOfView(std::move(other.fieldOfView))
, aspect(std::move(other.aspect))
, rotationSpeed(std::move(other.rotationSpeed))
, minVerticalRotation(std::move(other.minVerticalRotation))
, maxVerticalRotation(std::move(other.maxVerticalRotation))
, position(std::move(other.position)) 
, rotation(std::move(other.rotation))
, direction(std::move(other.direction))
, up(std::move(other.up))
, right(std::move(other.right))
, viewMatrix(std::move(other.viewMatrix))
, projectionMatrix(std::move(other.projectionMatrix))
, basicModelMatrix(std::move(other.basicModelMatrix))
, MVP(std::move(other.MVP))
{}

oglu::Camera& oglu::Camera::operator=(const Camera& other){
    window = other.window;
    radius = other.radius;
    fieldOfView = other.fieldOfView;
    rotationSpeed = other.rotationSpeed;
    minVerticalRotation = other.minVerticalRotation;
    maxVerticalRotation = other.maxVerticalRotation;
    position = other.position; 
    rotation = other.rotation;
    direction = other.direction;
    up = other.up;
    right = other.right;
    viewMatrix = other.viewMatrix;
    projectionMatrix = other.projectionMatrix;
    basicModelMatrix = other.basicModelMatrix;
    MVP = other.MVP;
    return *this;

}

oglu::Camera& oglu::Camera::operator=(Camera&& other){
    if (this != &other) {
        window = std::move(other.window);
        radius = std::move(other.radius);
        fieldOfView = std::move(other.fieldOfView);
        rotationSpeed = std::move(other.rotationSpeed);
        minVerticalRotation = std::move(other.minVerticalRotation);
        maxVerticalRotation = std::move(other.maxVerticalRotation);
        position = std::move(other.position); 
        rotation = std::move(other.rotation);
        direction = std::move(other.direction);
        up = std::move(other.up);
        right = std::move(other.right);
        viewMatrix = std::move(other.viewMatrix);
        projectionMatrix = std::move(other.projectionMatrix);
        basicModelMatrix = std::move(other.basicModelMatrix);
        MVP = std::move(other.MVP);
    }
    return *this;
}

oglu::Camera::~Camera(){

}

oglu::Camera::Camera(GLFWwindow* window)
: window(window)
, radius(0)
, fieldOfView(45.0f)
, aspect(0)
, rotationSpeed(0.001f)
, minVerticalRotation(-1.5)
, maxVerticalRotation(1.5)
, position(0,0,0) 
, rotation(0,0,0)
, direction()
, up()
, right()
, viewMatrix(1)
, projectionMatrix(1)
, basicModelMatrix(1)
, MVP(1)
{
    int xSize;
    int ySize;
    glfwGetFramebufferSize(window, &xSize, &ySize);
    aspect = (float)xSize/ySize;
    updateDirections();
}

void oglu::Camera::lookAt(const glm::vec3& point){
    	this->position = point - this->radius * getDirection();
}

void oglu::Camera::rotate(const double shift_x, const double shift_y){
    rotation[0] -= rotationSpeed * shift_x;
    rotation[1] += rotationSpeed * shift_y;
	if(rotation[1] > maxVerticalRotation) rotation[1] = maxVerticalRotation; 
	else if( rotation[1] < minVerticalRotation) rotation[1] = minVerticalRotation;
    updateDirections();
}


void oglu::Camera::moveToPosition(const glm::vec3& point){
    position = point;
}

void oglu::Camera::computeMatrices(){
    int xSize;
    int ySize;
    glfwGetFramebufferSize(window, &xSize, &ySize);
    //aspect = (float)xSize/ySize;
	projectionMatrix = glm::perspective(glm::radians(fieldOfView), aspect, 0.1f, 1000.0f);

    glm::vec3 r = -right;
    glm::vec3 d = -direction;
    viewMatrix[0][0] = r.x;
    viewMatrix[1][0] = r.y;
    viewMatrix[2][0] = r.z;
    viewMatrix[0][1] = up.x;
    viewMatrix[1][1] = up.y;
    viewMatrix[2][1] = up.z;
    viewMatrix[0][2] = d.x;
    viewMatrix[1][2] = d.y;
    viewMatrix[2][2] = d.z;
    viewMatrix[3][0] = -dot(r, position);
    viewMatrix[3][1] = -dot(up, position);
    viewMatrix[3][2] = -dot(d, position);
    MVP = projectionMatrix * viewMatrix * basicModelMatrix;

}

void oglu::Camera::computeMatricesFrom(glm::vec3& point){
    int xSize;
    int ySize;
    glfwGetFramebufferSize(window, &xSize, &ySize);
    //aspect = (float)xSize/ySize;
	projectionMatrix = glm::perspective(glm::radians(fieldOfView), aspect, 0.1f, 1000.0f);

    glm::vec3 r = -right;
    glm::vec3 d = -direction;
    viewMatrix[0][0] = r.x;
    viewMatrix[1][0] = r.y;
    viewMatrix[2][0] = r.z;
    viewMatrix[0][1] = up.x;
    viewMatrix[1][1] = up.y;
    viewMatrix[2][1] = up.z;
    viewMatrix[0][2] = d.x;
    viewMatrix[1][2] = d.y;
    viewMatrix[2][2] = d.z;
    viewMatrix[3][0] = -dot(r,  point);
    viewMatrix[3][1] = -dot(up, point);
    viewMatrix[3][2] = -dot(d,  point);
    MVP = projectionMatrix * viewMatrix * basicModelMatrix;

}

void oglu::Camera::updateDirections() {
    // Direction vector
    direction = glm::vec3(
		cosf(-rotation[1]) * cosf(rotation[0]),
		cosf(-rotation[1]) * sinf(rotation[0]),
		sinf(-rotation[1])
	);
    // Right vector
	right = -glm::vec3(cosf(rotation[0] - glm::half_pi<float>()), sinf(rotation[0] - glm::half_pi<float>()),0);
	// Up vector
	up = glm::cross( direction, right );
}

glm::vec3 oglu::Camera::getRotation() const noexcept {
    return rotation;
}

glm::vec3 oglu::Camera::getDirection() const noexcept {
    return direction;
}

glm::vec3 oglu::Camera::getRightVector() const noexcept {
    return right;
}

glm::vec3 oglu::Camera::getUpVector() const noexcept {
    return up;
}

const glm::mat4& oglu::Camera::getViewMatrix() const noexcept {
    return viewMatrix;
}

const glm::mat4& oglu::Camera::getProjectionMatrix() const noexcept {
    return projectionMatrix;
}

const glm::mat4& oglu::Camera::getMVP() const noexcept {
    return MVP;
}

void oglu::Camera::setRotation(const glm::vec3& _rotation){
    rotation = _rotation;
    updateDirections();
}


