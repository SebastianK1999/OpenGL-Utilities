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

#include <chrono>
#include <vector>
#include <GLFW/glfw3.h>


namespace oglu
{
    typedef std::vector<int> KeyBundle;

    enum EKeyState
    {
        EKeyState_undefined = 0,
        EKeyState_released,
        EKeyState_isJustPressed,
        EKeyState_pressed,
        EKeyState_isJustReleased,
    };


    class KeyTracker
    {
    private:
        class KeyData
        {
        public:
            KeyData(const KeyData& other) noexcept;
            KeyData(KeyData&& other) noexcept;
            KeyData& operator=(const KeyData& other) noexcept;
            KeyData& operator=(KeyData&& other) noexcept;
            virtual ~KeyData() noexcept;
            KeyData(const int key, const EKeyState state, const std::chrono::_V2::system_clock::time_point stateChangeTime) noexcept;
            
            int key;
            EKeyState state;
            std::chrono::_V2::system_clock::time_point stateChangeTime;
        };

    public:
        KeyTracker(const KeyTracker& other) noexcept;
        KeyTracker(KeyTracker&& other) noexcept;
        KeyTracker& operator=(const KeyTracker& other) noexcept;
        KeyTracker& operator=(KeyTracker&& other) noexcept;
        virtual ~KeyTracker() noexcept;
        KeyTracker() noexcept; 

        void update(GLFWwindow* window);
        void update(GLFWwindow* window, const int key);
        void update(GLFWwindow* window, const KeyBundle& keyBundle);
        void addKey(const int key) noexcept;
        void addKey(const KeyBundle& keyBundle) noexcept;
        bool hasKey(const int key) const;
        bool hasKeyBundle(const KeyBundle& keyBundle) const;
        std::vector<bool> hasKey(const KeyBundle& keyBundle) const;
        void removeKey(const int key);
        void removeKey(const KeyBundle& keyBundle);
        EKeyState getState(const int key) const;
        EKeyState getState(const KeyBundle& keyBundle) const;
        bool isReleased(const int key) const;
        bool isReleased(const KeyBundle& keyBundle) const;
        bool isJustPressed(const int key) const;
        bool isJustPressed(const KeyBundle& keyBundle) const;
        bool isPressed(const int key) const;
        bool isPressed(const KeyBundle& keyBundle) const;
        bool isJustReleased(const int key) const;
        bool isJustReleased(const KeyBundle& keyBundle) const;
        int getTime(const int key) const;
        int getTime(const KeyBundle& keyBundle) const;

    private:
        std::vector<KeyData> trackedKeys;

        void update(GLFWwindow* window, KeyData& keyData);
    };
}