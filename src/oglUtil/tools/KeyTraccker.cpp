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
* SOFTWARE.*/


#include "oglUtil/tools/KeyTracker.hpp"
#include <algorithm>

// TODO - glfwGetTime() instead of std::chrono which does not work on Windows
oglu::KeyTracker::KeyData::KeyData(const KeyData& other) noexcept:
    key(other.key),
    state(other.state),
    stateChangeTime(other.stateChangeTime)
{}

oglu::KeyTracker::KeyData::KeyData(KeyData&& other) noexcept:
    key(std::move(other.key)),
    state(std::move(other.state)),
    stateChangeTime(std::move(other.stateChangeTime))
{}

oglu::KeyTracker::KeyData& oglu::KeyTracker::KeyData::operator=(const KeyData& other) noexcept {
    key = other.key;
    state = other.state;
    stateChangeTime = other.stateChangeTime;
    return *this;
}

oglu::KeyTracker::KeyData& oglu::KeyTracker::KeyData::operator=(KeyData&& other) noexcept {
    if (this != &other) {
        key = std::move(other.key);
        state = std::move(other.state);
        stateChangeTime = std::move(other.stateChangeTime);
    }
    return *this;
}

oglu::KeyTracker::KeyData::~KeyData() noexcept {

}

oglu::KeyTracker::KeyData::KeyData(const int key, const EKeyState state, const std::chrono::_V2::system_clock::time_point stateChangeTime) noexcept:
    key(key),
    state(state),
    stateChangeTime(stateChangeTime)
{

}

oglu::KeyTracker::KeyTracker(const KeyTracker& other) noexcept:
    trackedKeys(other.trackedKeys)
{}

oglu::KeyTracker::KeyTracker(KeyTracker&& other) noexcept:
    trackedKeys(std::move(other.trackedKeys))
{}

oglu::KeyTracker& oglu::KeyTracker::operator=(const KeyTracker& other) noexcept {
    trackedKeys = other.trackedKeys;
    return *this;

}

oglu::KeyTracker& oglu::KeyTracker::operator=(KeyTracker&& other) noexcept {
    if (this != &other) {
        trackedKeys = std::move(other.trackedKeys);
    }
    return *this;
}

oglu::KeyTracker::~KeyTracker() noexcept {

}

oglu::KeyTracker::KeyTracker() noexcept:
    trackedKeys()
{

}

void oglu::KeyTracker::update(GLFWwindow* window){
    for (std::vector<KeyData>::iterator keyIterator = trackedKeys.begin(); keyIterator != trackedKeys.end(); ++keyIterator){
        oglu::KeyTracker::update(window, *keyIterator);
    }
}

void oglu::KeyTracker::update(GLFWwindow* window, const int key){
    std::vector<KeyData>::iterator keyIterator = std::find_if(trackedKeys.begin(), trackedKeys.end(), [&key](const KeyData& ki){return (ki.key == key);});
    oglu::KeyTracker::update(window, *keyIterator);
}

void oglu::KeyTracker::update(GLFWwindow* window,const KeyBundle& keyBundle){
    for (KeyBundle::const_iterator keyIterator = keyBundle.begin(); keyIterator != keyBundle.end(); ++keyIterator){
        oglu::KeyTracker::update(window, *keyIterator);
    }
}

void oglu::KeyTracker::addKey(const int key) noexcept {
    std::vector<KeyData>::const_iterator keyIterator = std::find_if(trackedKeys.begin(), trackedKeys.end(), [&key](const KeyData& ki){return (ki.key == key);});
    if(keyIterator == trackedKeys.end()){
        trackedKeys.push_back(KeyData(key, EKeyState_released, std::chrono::high_resolution_clock::now()));
    }
}

void oglu::KeyTracker::addKey(const KeyBundle& keyBundle) noexcept {
    for (KeyBundle::const_iterator keyIterator = keyBundle.begin(); keyIterator != keyBundle.end(); ++keyIterator){
        oglu::KeyTracker::addKey(*keyIterator);
    }
}

bool oglu::KeyTracker::hasKey(const int key) const {
    std::vector<KeyData>::const_iterator keyIterator = std::find_if(trackedKeys.begin(), trackedKeys.end(), [&key](const KeyData& ki){return (ki.key == key);});
    return keyIterator != trackedKeys.end();
}

bool oglu::KeyTracker::hasKeyBundle(const KeyBundle& keyBundle) const {
    for (KeyBundle::const_iterator keyIterator = keyBundle.begin(); keyIterator != keyBundle.end(); ++keyIterator){
        if(!oglu::KeyTracker::hasKey(*keyIterator)){
            return false;
        }
    }
    return true;
}

std::vector<bool> oglu::KeyTracker::hasKey(const KeyBundle& keyBundle) const {
    std::vector<bool> hasKeys(keyBundle.size());
    for (KeyBundle::const_iterator keyIterator = keyBundle.begin(); keyIterator != keyBundle.end(); ++keyIterator){
        hasKeys.emplace_back(oglu::KeyTracker::hasKey(*keyIterator));
    }
    return hasKeys;
}


void oglu::KeyTracker::removeKey(const int key){
    std::vector<KeyData>::iterator keyIterator = std::find_if(trackedKeys.begin(), trackedKeys.end(), [&key](const KeyData& ki){return (ki.key == key);});
    if(keyIterator != trackedKeys.end()){
        keyIterator = trackedKeys.erase(keyIterator);
    }
}

void oglu::KeyTracker::removeKey(const KeyBundle& keyBundle){
    for (KeyBundle::const_iterator keyIterator = keyBundle.begin(); keyIterator != keyBundle.end(); ++keyIterator){
        oglu::KeyTracker::removeKey(*keyIterator);
    }
}

oglu::EKeyState oglu::KeyTracker::getState(const int key) const {
    std::vector<KeyData>::const_iterator keyIterator = std::find_if(trackedKeys.begin(), trackedKeys.end(), [&key](const KeyData& ki){return (ki.key == key);});
    if(keyIterator != trackedKeys.end()){
        return keyIterator->state;
    }
    return EKeyState_undefined;
}

oglu::EKeyState oglu::KeyTracker::getState(const KeyBundle& keyBundle) const {
    int released_count = 0;
    int jPressed_count = 0;
    int pressed_count = 0;
    int jReleased_count = 0;
    for (KeyBundle::const_iterator keyIterator = keyBundle.begin(); keyIterator != keyBundle.end(); ++keyIterator){
        switch (getState(*keyIterator))
        {
        case EKeyState_released:
            released_count++;
            break;
        case EKeyState_isJustPressed:
            jPressed_count++;
            break;
        case EKeyState_pressed:
            pressed_count++;
            break;
        case EKeyState_isJustReleased:
            jReleased_count++;
            break;
        default:
            return EKeyState_undefined;
        }
    }
    if(keyBundle.size() == pressed_count){
        return EKeyState_pressed;
    }
    else if(keyBundle.size() == pressed_count + jPressed_count){
        return EKeyState_isJustPressed;
    }
    else if(keyBundle.size() == pressed_count + jReleased_count){
        return EKeyState_isJustReleased;
    }
    return EKeyState_released;
}

bool oglu::KeyTracker::isReleased(const int key) const {
    EKeyState state = getState(key);
    return state == EKeyState_released || state == EKeyState_isJustReleased ;
}

bool oglu::KeyTracker::isReleased(const KeyBundle& keyBundle) const {
    EKeyState state = getState(keyBundle);
    return state == EKeyState_released || state == EKeyState_isJustReleased ;
    
}

bool oglu::KeyTracker::isJustPressed(const int key) const {
    return getState(key) == EKeyState_isJustPressed;
    
}

bool oglu::KeyTracker::isJustPressed(const KeyBundle& keyBundle) const {
    return getState(keyBundle) == EKeyState_isJustPressed;
    
}

bool oglu::KeyTracker::isPressed(const int key) const {
    EKeyState state = getState(key);
    return state == EKeyState_pressed || state == EKeyState_isJustPressed;
    
}

bool oglu::KeyTracker::isPressed(const KeyBundle& keyBundle) const {
    EKeyState state = getState(keyBundle);
    return state == EKeyState_pressed || state == EKeyState_isJustPressed;
    
}

bool oglu::KeyTracker::isJustReleased(const int key) const {
    return getState(key) == EKeyState_isJustReleased;
    
}

bool oglu::KeyTracker::isJustReleased(const KeyBundle& keyBundle) const {
    return getState(keyBundle) == EKeyState_isJustReleased;
    
}

int oglu::KeyTracker::getTime(const int key) const {
    std::vector<KeyData>::const_iterator keyIterator = std::find_if(trackedKeys.begin(), trackedKeys.end(), [&key](const KeyData& ki){return (ki.key == key);});
    if(keyIterator != trackedKeys.end()){
        return std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - keyIterator->stateChangeTime).count();
    }
    return -1;
}

int oglu::KeyTracker::getTime(const KeyBundle& keyBundle) const {
    return false;
}

void oglu::KeyTracker::update(GLFWwindow* window, KeyData& keyData){
    if(&keyData != &*(trackedKeys.end())){
        switch (keyData.state)
        {
        default:
        case EKeyState_released:
            if(glfwGetKey( window, keyData.key ) == GLFW_PRESS){
                keyData.state = EKeyState_isJustPressed;
                keyData.stateChangeTime = std::chrono::high_resolution_clock::now();
            }
            break;
        case EKeyState_isJustPressed:
            if(glfwGetKey( window, keyData.key ) == GLFW_PRESS){
                keyData.state = EKeyState_pressed;
            }
            else {
                keyData.state = EKeyState_isJustReleased;
                keyData.stateChangeTime = std::chrono::high_resolution_clock::now();
            }
            break;
        case EKeyState_pressed:
            if(! (glfwGetKey( window, keyData.key ) == GLFW_PRESS)){
                keyData.state = EKeyState_isJustReleased;
                keyData.stateChangeTime = std::chrono::high_resolution_clock::now();
            }
            break;
        case EKeyState_isJustReleased:
            if (! (glfwGetKey( window, keyData.key ) == GLFW_PRESS)){
                keyData.state = EKeyState_released;
            }
            else {
                keyData.state = EKeyState_isJustPressed;
                keyData.stateChangeTime = std::chrono::high_resolution_clock::now();
            }
            break;
        }
    }
}