#include "oglUtil_OLD/Line.hpp"

Line::Line(double_t X, double_t Y, GLfloat r,GLfloat g, GLfloat b, GLfloat a) : 
    DrawableBase(0),
    scale(1.0),
    x(X), y(Y),
    circle_color_0{r,g,b,a},
    circle_color_1{r,g,b,a},
    rotation(0.75)
{
    setShaders();
}

Line::Line(double_t X, double_t Y, GLfloat r,GLfloat g, GLfloat b)  : Line(X,Y,r,g,b,1.0) {}
Line::Line(double_t X, double_t Y, GLfloat c, GLfloat a) : Line(X,Y, c,c,c,a){}
Line::Line(double_t X, double_t Y, GLfloat c) : Line(X,Y, c,c,c,1.0){}
Line::Line(double_t X, double_t Y) : Line(X,Y,1.0,1.0,1.0,1.0){}

void Line::setRotation(double_t alpha){
    this->rotation = (alpha > M_PI * 2? alpha - M_PI*2 : alpha);
}

void Line::addRotation(double_t alpha){
    this->setRotation(this->rotation + alpha);
}

const double_t Line::getRotation() const{
    return this->rotation;
}

void Line::setShaders() {
    compileShaders(R"END(

        #version 330 
        #extension GL_ARB_explicit_uniform_location : require
        #extension GL_ARB_shading_language_420pack : require
        layout(location = 0) uniform float scale;
        layout(location = 1) uniform float rotation;
        layout(location = 2) uniform vec2  center;
        layout(location = 3) uniform vec4  circle_color_0;
        layout(location = 4) uniform vec4  circle_color_1;

        out vec4 vColor;
        out vec2 vPos;

        void main(void) {
            const vec2 vertices[] = vec2[2](vec2( cos(rotation), sin(rotation)),
                                             vec2(-cos(rotation), -sin(rotation)));
            const vec4 colors[]    = vec4[2](vec4(circle_color_0),
                                             vec4(circle_color_1));

            vColor      = colors[gl_VertexID];
            gl_Position = vec4(vertices[gl_VertexID] * scale + center, 0.5, 1.0); 
            vPos = vertices[gl_VertexID];
         }

      )END", R"END(

         #version 330 
         
         in  vec4 vColor;
         in  vec2 vPos;
         out vec4 color;

         vec2 coord = vPos;

         void main(void) {
            color = vColor;
         } 

      )END");
}

void Line::draw() {
    bindProgram();
    bindBuffers();
    glUniform1f(0, this->scale);  // scale  in vertex shader
    glUniform1f(1, this->rotation);
    glUniform2f(2, this->x, this->y);  // center in vertex shader
    glUniform4f(3, circle_color_0[0],circle_color_0[1],circle_color_0[2],circle_color_0[3]);
    glUniform4f(4, circle_color_1[0],circle_color_1[1],circle_color_1[2],circle_color_1[3]);

    glDrawArrays(GL_LINES, 0, 3);
}


void Line::setColorHead(float r, float g, float b, float a){
    circle_color_0[0]=r;circle_color_0[1]=g;circle_color_0[2]=b;circle_color_0[3]=a;
}

void Line::setColorTail(float r, float g, float b, float a){
    circle_color_1[0]=r;circle_color_1[1]=g;circle_color_1[2]=b;circle_color_1[3]=a;
}

void Line::setColor(float r, float g, float b, float a){
    circle_color_0[0]=r;circle_color_0[1]=g;circle_color_0[2]=b;circle_color_0[3]=a;
    circle_color_1[0]=r;circle_color_1[1]=g;circle_color_1[2]=b;circle_color_1[3]=a;
}
