
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


#include <iostream>
#include <sstream>
#include <math.h>
#include <ctime>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "oglUtil/oglu.hpp"

#include "oglUtil_OLD/Misc.hpp"
#include "oglUtil_OLD/controls.hpp"
#include "oglUtil_OLD/objloader.hpp"
#include "oglUtil_OLD/Cube.hpp"
#include "oglUtil_OLD/SpheresInstances.hpp"
#include "oglUtil_OLD/Sphere.hpp"

#include "whereami.h"



/*
* Because of unknown reasons, these can not be a member of MainWindow Class
* It can't initalize ArrayBuffer while Winodw is constructing
*/
Cube* cubePtr;
SpheresInstances* obsticles;
Sphere* playerPtr;


class MainWindow 
: public oglu::Window 
{
public:
    MainWindow(const int _width, const int _height, const std::string& _name);
    ~MainWindow();
    void mainFunction() override;
    void drawFunction() override;
    
    void moveOjects();
    void colisions();

    bool switchCamera;
    bool vievMainCamera;
    int levelNumer;
    int gridSize;

    MovingViewPort camera;
    MovingViewPort camera2;

    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    glm::mat4 ModelMatrix;
    glm::mat4 MVP;
};

MainWindow::~MainWindow(){

}

MainWindow::MainWindow(const int _width, const int _height, const std::string& _name) 
: oglu::Window(_width, _height, _name, false)
, switchCamera(0)
, vievMainCamera(true)
, levelNumer(1)
, gridSize(3)
, camera(getGLFWwindowPtr())
, camera2(getGLFWwindowPtr())
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    
    camera.radius = 5.0f/gridSize;
    camera.speed = 5.0f/gridSize;
    camera.computeMatricesFromInputs(); 
    camera2.speed = 5.0f/gridSize;
    camera2.computeMatricesFromInputs(); 
}

void MainWindow::mainFunction(){
    if(keyTracker.isJustPressed(GLFW_KEY_F)){
        std::cout << "Respect paid!\n";
    }
    if(keyTracker.isJustPressed(GLFW_KEY_F11)){
        switchFullScreen();
    }
    if(keyTracker.isJustReleased(oglu::KeyBundle{GLFW_KEY_F, GLFW_KEY_F11})){
        std::cout << "Wait, that's illegal!\n";
    }
    if (keyTracker.isJustPressed(GLFW_KEY_TAB)){
        vievMainCamera = !vievMainCamera;
    } 
    moveOjects();
    colisions();
}

void MainWindow::drawFunction(){
    Cube& cube = *cubePtr;
    Sphere& player = *playerPtr;

    cube.draw(MVP);
    obsticles->draw(MVP,player.pos);
    glClear(GL_DEPTH_BUFFER_BIT);
    player.draw(MVP);
}

void MainWindow::moveOjects(){
    Sphere& player = *playerPtr;

    if(vievMainCamera){
        ProjectionMatrix = camera.getProjectionMatrix();
        ViewMatrix = camera.getViewMatrix();
        ModelMatrix = glm::mat4(1.0);
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        //allows moving in other view
        camera.computeMatricesFromInputs(player.pos);
        obsticles->move(camera.getTime());
    }
    else
    {
        ProjectionMatrix = camera2.getProjectionMatrix();
        ViewMatrix = camera2.getViewMatrix();
        ModelMatrix = glm::mat4(1.0);
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        //allows moving in other view
        camera2.computeMatricesFromInputs(player.pos);
        obsticles->move(camera2.getTime());
    }
}

void MainWindow::colisions(){
    Cube& cube = *cubePtr;
    Sphere& player = *playerPtr;

    if(player.pos[2] + player.scale[2] > cube.scale[2]){
        player.pos[2] = -1.5;
        levelNumer++;
        gridSize++;
        delete obsticles;
        obsticles = new SpheresInstances(gridSize);
        player.scale[0] = 0.5 / gridSize;
        player.scale[1] = 0.5 / gridSize;
        player.scale[2] = 0.5 / gridSize;
        camera.radius = 5.0f/gridSize;
        camera.speed = 5.0f/gridSize;
        camera2.speed = 5.0f/gridSize;

    }
    if(abs(player.pos[0]) + player.scale[0] > cube.scale[0]) player.pos[0] = (player.pos[0] > 0 ? cube.scale[0] - player.scale[0] : -cube.scale[0] + player.scale[0]);
    if(abs(player.pos[1]) + player.scale[1] > cube.scale[1]) player.pos[1] = (player.pos[1] > 0 ? cube.scale[1] - player.scale[1] : -cube.scale[1] + player.scale[1]);
    if(abs(player.pos[2]) + player.scale[2] > cube.scale[2]) player.pos[2] = (player.pos[2] > 0 ? cube.scale[2] - player.scale[2] : -cube.scale[2] + player.scale[2]);

    for(size_t i = 0; i < obsticles->sizeInstances; i++){
        if(
            abs(player.pos[0] - obsticles->pos[i][0]) < player.scale[0] + obsticles->scale[i][0] &&
            abs(player.pos[1] - obsticles->pos[i][1]) < player.scale[1] + obsticles->scale[i][1] &&
            abs(player.pos[2] - obsticles->pos[i][2]) < player.scale[2] + obsticles->scale[i][2] 
        ){
            double horAngle = std::atan((player.pos[2] - obsticles->pos[i][2]) / abs(player.pos[0] - obsticles->pos[i][0]));
            if(player.pos[0] < obsticles->pos[i][0]){
                horAngle = M_PI - horAngle;
            }

            double horR = std::sqrt(pow(player.pos[0] - obsticles->pos[i][0], 2) + pow(player.pos[2] - obsticles->pos[i][2], 2));
            double verAngle = std::atan((player.pos[1] - obsticles->pos[i][1]) / horR);
            
            if(
                abs(player.pos[0]-obsticles->pos[i][0]) < abs(cos(horAngle) * (player.scale[0] + obsticles->scale[i][0]) * cos(verAngle))   &&
                abs(player.pos[1]-obsticles->pos[i][1]) < abs(sin(verAngle) * (player.scale[1] + obsticles->scale[i][1])                )   &&
                abs(player.pos[2]-obsticles->pos[i][2]) < abs(sin(horAngle) * (player.scale[2] + obsticles->scale[i][2]) * cos(verAngle))
            ){
                player.pos[0] = obsticles->pos[i][0] + cos(horAngle) * (player.scale[0] + obsticles->scale[i][0]) * cos(verAngle);
                player.pos[1] = obsticles->pos[i][1] + sin(verAngle) * (player.scale[1] + obsticles->scale[i][1]);
                player.pos[2] = obsticles->pos[i][2] + sin(horAngle) * (player.scale[2] + obsticles->scale[i][2]) * cos(verAngle);

            }
        }
    }
}


int main(int argc, char *argv[]) {
    int dirname_length;
    int length = wai_getExecutablePath(NULL, 0, NULL);
    std::vector<char> path(length+1);
    wai_getExecutablePath(path.data() , length, &dirname_length);
    path.data()[length] = '\0';
    std::cout << path.data() << "\n";

    oglu::setupOglu();
    oglu::setupGlfw();
    MainWindow window(800, 800, "Bubbles");
    oglu::setupGlew(false);
    
    window.keyTracker.addKey(GLFW_KEY_F);
    window.keyTracker.addKey(GLFW_KEY_F11);
    window.keyTracker.addKey(GLFW_KEY_TAB);

    int gridSize = 3;
    if(argc > 1){ 
        std::istringstream ss(argv[1]);
        if (!(ss >> gridSize)) {
            std::cerr << "Invalid number: " << argv[1] << '\n';
        } else if (!ss.eof()) {
            std::cerr << "Trailing characters after number: " << argv[1] << '\n';
        }
    }
    
    /*
    * Because of unknown reasons, these can not be a member of MainWindow Class
    */
    // SpheresInstances* obsticles;
    Cube cube;
    Sphere player(15);
    cubePtr = &cube;
    obsticles = new SpheresInstances(gridSize);
    playerPtr = &player;
    cube.scale[2] = 2;
    player.scale[0] = 0.5 / gridSize;
    player.scale[1] = 0.5 / gridSize;
    player.scale[2] = 0.5 / gridSize;
    player.pos[0] = (1.0f/gridSize)-1;
    player.pos[1] = (1.0f/gridSize)-1;
    player.pos[2] = (1.0f/gridSize)-1;

    time_t start = time(0);
    window.mainLoop();


    int seconds_since_start = difftime( time(0), start);
    std::cout<< "Time played: "  << seconds_since_start << "[s]\n" 
            << "You got to level" << window.levelNumer << "\n";

    delete obsticles;
    return 0;
}