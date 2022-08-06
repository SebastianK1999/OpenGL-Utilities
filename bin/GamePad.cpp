#include <iostream>
#include <iomanip>
#include <vector>

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

MainWindow::~MainWindow(){

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
    int countAx;
    int countBtn;
    const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &countAx);
    const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &countBtn);
    if(countAx > 0){
        for(int i = 0; i < countAx; i++){
            std::cout<< "ax[" << i << "]:" << std::setfill('0') << std::setw(10) << std::left << axes[i] << "\t";
            
        }
    }
    std::cout << "\r\n";
    if(countBtn > 0){
        for(int i = 0; i < countBtn; i++){
            std::cout << "btn[" << i << "]:" << buttons[i] + 0 << " ";
            
        }
    }
    std::cout << "\r\n\x1b[0;0H\x1b[2J";

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