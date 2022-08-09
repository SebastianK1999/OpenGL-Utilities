#include "oglUtil_OLD/Background.hpp"

Background::Background() : 
    DrawableBase(0),
    xs{Misc::rand::get().Decim(-0.9,0.9), Misc::rand::get().Decim(-0.9,0.9), Misc::rand::get().Decim(-0.9,0.9)},
    ys{Misc::rand::get().Decim(-0.9,0.9), Misc::rand::get().Decim(-0.9,0.9), Misc::rand::get().Decim(-0.9,0.9)},
    alphas{Misc::rand::get().Decim(2*M_PI), Misc::rand::get().Decim(2*M_PI), Misc::rand::get().Decim(2*M_PI)}
{
    setShaders();
}
void Background::setShaders() {
        compileShaders(R"END(

        #version 330 
        #extension GL_ARB_explicit_uniform_location : require
        #extension GL_ARB_shading_language_420pack : require
        out vec3 vPos;

		layout(location = 12)uniform mat4 MVP;
         
        void main(void) {
        const vec3 vertices[] = vec3[5](vec3(-0.9, -0.9, 0),
                                            vec3(0.9, -0.9, 0),
                                            vec3(0.9,  0.9, 0),
                                            vec3(-0.9,  0.9, 0),
                                            vec3(-0.9,  0, 0));
        gl_Position = MVP * vec4(vertices[gl_VertexID], 1.0); 
        vPos = vertices[gl_VertexID];
        }

    )END", R"END(

        #version 330 
        #extension GL_ARB_explicit_uniform_location : require
        #extension GL_ARB_shading_language_420pack : require
        
        layout(location = 10) uniform vec3  xs;
        layout(location = 11) uniform vec3  ys;

        in  vec3 vPos;
        out vec4 color;

        void main(void) {
            color = vec4(
                (1 - sqrt( pow(vPos.x - xs[0], 2) + pow(vPos.y - ys[0],2) )) * 0.6  ,
                (1 - sqrt( pow(vPos.x - xs[1], 2) + pow(vPos.y - ys[1],2) )) * 0.6  ,
                (1 - sqrt( pow(vPos.x - xs[2], 2) + pow(vPos.y - ys[2],2) )) * 0.6  ,
                1);
        } 

    )END");
}
void Background::draw(glm::mat4& MVP) {
    bindProgram();
    bindBuffers();
    glUniform3f(10, xs[0],xs[1],xs[2]);
    glUniform3f(11, ys[0],ys[1],ys[2]);


    glUniformMatrix4fv(12, 1, GL_FALSE, &MVP[0][0]);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

    this->move();
}

void Background::move(){
    for(int i = 0; i < 3; i++ ){
        xs[i] += cos(alphas[i]) * 0.01;
        ys[i] += sin(alphas[i]) * 0.01;
        if(xs[i] > 0.9 || xs[i] < -0.9){
            alphas[i] = M_PI - alphas[i];
        }
        else if(ys[i] > 0.9 || ys[i] < -0.9){
            alphas[i] = - alphas[i];   
        }
    }
}