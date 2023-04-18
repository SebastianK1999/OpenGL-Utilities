#include <iostream>

#include <GL/glew.h>
#include "oglUtil/ogluTools.hpp"

#include <glm/glm.hpp>



class MainWindow 
: public oglu::Window 
{
public:
    MainWindow(const int _width, const int _height, const std::string& _name);
    ~MainWindow();
    void mainFunction() override;
    void drawFunction() override;
};

MainWindow::~MainWindow()
{
}

MainWindow::MainWindow(const int _width, const int _height, const std::string& _name) 
: oglu::Window(_width, _height, _name, false)
{
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
}

void MainWindow::drawFunction(){

}

int main(int argc, char *argv[]) {
    oglu::setupOglu();
    oglu::setupGlfw();
    MainWindow window(800, 600, "Empty window");
    oglu::setupGlew(false);
    
    window.keyTracker.addKey(GLFW_KEY_F);
    window.keyTracker.addKey(GLFW_KEY_F11);
    
    window.mainLoop();

    return 0;
}