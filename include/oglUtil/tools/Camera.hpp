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


#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace oglu
{
    class Camera
    {
    public:
        GLFWwindow* window;
        float radius;
        float fieldOfView;
        float aspect; 
        double rotationSpeed;
        double minVerticalRotation;
        double maxVerticalRotation;
        glm::vec3 position; 

        Camera(const Camera& other);
        Camera(Camera&& other);
        Camera& operator=(const Camera& other);
        Camera& operator=(Camera&& other);
        virtual ~Camera();
        Camera(GLFWwindow *window);

        void lookAt(const glm::vec3& point);
        void rotate(const double shift_x, const double shift_y);
        void moveToPosition(const glm::vec3& point);
        void computeMatrices();
        void computeMatricesFrom(glm::vec3& point);
        void updateDirections();
        glm::vec3 getRotation() const noexcept;
        glm::vec3 getDirection() const noexcept;
        glm::vec3 getRightVector() const noexcept;
        glm::vec3 getUpVector() const noexcept;
        const glm::mat4& getViewMatrix() const noexcept;
        const glm::mat4& getProjectionMatrix() const noexcept;
        const glm::mat4& getMVP() const noexcept;
        void setRotation(const glm::vec3& rotation);

    private:
        glm::vec3 rotation;
        glm::vec3 direction;
        glm::vec3 up;
        glm::vec3 right;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::mat4 basicModelMatrix;
        glm::mat4 MVP;

    };
}
