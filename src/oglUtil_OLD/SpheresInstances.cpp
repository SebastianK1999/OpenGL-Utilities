#include "oglUtil_OLD/SpheresInstances.hpp"




void SpheresInstances::move(float dt){
    for(int i = 0; i < sizeInstances; i++){
        this->pos[i][1] += this->scale[i][1]*8*dt;
        this->scale[i][0] *= 1.0+1e-3;
        this->scale[i][1] *= 1.0+1e-3;
        this->scale[i][2] *= 1.0+1e-3;
        if(this->pos[i][1] - this->scale[i][1] > 1){
            float sc = Misc::rand::get().Decim(1/(double)N, 0.5/(double)N);
            this->scale[i][0] = sc;
            this->scale[i][1] = sc;
            this->scale[i][2] = sc;
            this->pos[i][0] = Misc::rand::get().Decim( -1 + this->scale[i][0], 1 - this->scale[i][0] );
            this->pos[i][1] = -1 - this->scale[i][1];
            this->pos[i][2] = Misc::rand::get().Decim( -1 + this->scale[i][2], 1 - this->scale[i][2] );
        }
    }
}


std::vector<GLfloat> SpheresInstances::generateMesh(int N){

    GLfloat radius = 1;
    // clear memory of prev arrays
    std::vector<GLfloat> vertices;

    GLfloat sectorStep = (GLfloat) M_PI /(GLfloat) N;
    GLfloat stackStep = (GLfloat) M_PI / (GLfloat) N;

    for(int i = 0; i < N; ++i)
    {
        GLfloat stackAngle = i * stackStep - M_PI/2;

        for(int j = 0; j < N*2; ++j)
        {
            GLfloat sectorAngle = j * sectorStep;           // starting from 0 to 2pi
            std::vector<GLfloat> tempVer;

            if(i != 0){
                tempVer.push_back(radius * cosf(stackAngle) * cosf(sectorAngle));
                tempVer.push_back(radius * sinf(stackAngle));
                tempVer.push_back(radius * cosf(stackAngle) * sinf(sectorAngle));

                tempVer.push_back(radius * cosf(stackAngle + stackStep) * cosf(sectorAngle));
                tempVer.push_back(radius * sinf(stackAngle + stackStep));
                tempVer.push_back(radius * cosf(stackAngle + stackStep) * sinf(sectorAngle));

                tempVer.push_back(radius * cosf(stackAngle) * cosf(sectorAngle + sectorStep));
                tempVer.push_back(radius * sinf(stackAngle));
                tempVer.push_back(radius * cosf(stackAngle) * sinf(sectorAngle + sectorStep));
            }
            if(i != (N-1)){
                tempVer.push_back(radius * cosf(stackAngle) * cosf(sectorAngle + sectorStep));
                tempVer.push_back(radius * sinf(stackAngle));
                tempVer.push_back(radius * cosf(stackAngle) * sinf(sectorAngle + sectorStep));

                tempVer.push_back(radius * cosf(stackAngle + stackStep) * cosf(sectorAngle));
                tempVer.push_back(radius * sinf(stackAngle + stackStep));
                tempVer.push_back(radius * cosf(stackAngle + stackStep) * sinf(sectorAngle));

                tempVer.push_back(radius * cosf(stackAngle + stackStep) * cosf(sectorAngle + sectorStep));
                tempVer.push_back(radius * sinf(stackAngle + stackStep));
                tempVer.push_back(radius * cosf(stackAngle + stackStep) * sinf(sectorAngle + sectorStep));
            }

            vertices.insert(vertices.end(), tempVer.begin(), tempVer.end());
        }
    }

    return vertices;
}

std::vector<GLfloat> SpheresInstances::generateNormals(int N, std::vector<GLfloat> points){
    std::vector<GLfloat> normals;
    normals.reserve(points.size());
    for(int i = 0; i < points.size(); i += 9)
    {
        glm::vec3 p1(
            points[i],
            points[i+1],
            points[i+2]
        );
        glm::vec3 p2(
            points[i+3],
            points[i+4],
            points[i+5]
        );
        glm::vec3 p3(
            points[i+6],
            points[i+7],
            points[i+8]
        );

        /*
        glm::vec3 V = (p1-p2);
        glm::vec3 U = (p3-p2);

        glm::vec3 Norm;
        Norm.x =  U.y*V.z - U.z*V.y;
        Norm.y =  U.z*V.x - U.x*V.z;
        Norm.z =  U.x*V.y - U.y*V.x;
        
        GLfloat len = std::sqrt(Norm.x*Norm.x + Norm.y*Norm.y + Norm.z*Norm.z);
        Norm.x = Norm.x / len;
        Norm.y = Norm.y / len;
        Norm.z = Norm.z / len;
        */


        glm::vec3 Norm = p1;
        GLfloat len = std::sqrt(Norm.x*Norm.x + Norm.y*Norm.y + Norm.z*Norm.z);
        Norm.x = Norm.x / len;
        Norm.y = Norm.y / len;
        Norm.z = Norm.z / len;
       
        normals.emplace_back(Norm.x),
        normals.emplace_back(Norm.y),
        normals.emplace_back(Norm.z),
        
        Norm = p2;
        len = std::sqrt(Norm.x*Norm.x + Norm.y*Norm.y + Norm.z*Norm.z);
        Norm.x = Norm.x / len;
        Norm.y = Norm.y / len;
        Norm.z = Norm.z / len;
        
        normals.emplace_back(Norm.x);
        normals.emplace_back(Norm.y);
        normals.emplace_back(Norm.z);

        Norm = p3;
        len = std::sqrt(Norm.x*Norm.x + Norm.y*Norm.y + Norm.z*Norm.z);
        Norm.x = Norm.x / len;
        Norm.y = Norm.y / len;
        Norm.z = Norm.z / len;
        
        normals.emplace_back(Norm.x);
        normals.emplace_back(Norm.y);
        normals.emplace_back(Norm.z);
    }
    return normals;
}

//first and las tolumn are swaped, now culling shows only inside of cube;
SpheresInstances::SpheresInstances(const size_t& N) :
    g_vertex_buffer_data(generateMesh(20)),
    g_normal_buffer_data(generateNormals(20, g_vertex_buffer_data)),
    sizeInstances(N*N),
    N(N)
{
    pos = new glm::vec3[sizeInstances];
    scale = new glm::vec3[sizeInstances];

    for(int i = 0; i < sizeInstances; i++){
        float sc = Misc::rand::get().Decim(1/(double)N, 0.5/(double)N);
        this->scale[i][0] = sc;
        this->scale[i][1] = sc;
        this->scale[i][2] = sc;
        this->pos[i][0] = Misc::rand::get().Decim( -1 + this->scale[i][0], 1 - this->scale[i][0] );
        this->pos[i][1] = Misc::rand::get().Decim( -1 + this->scale[i][1], 1 - this->scale[i][1] );
        this->pos[i][2] = Misc::rand::get().Decim( -1 + this->scale[i][2], 1 - this->scale[i][2] );
        
    }
    
    setShaders();
    setBuffers();

}


SpheresInstances::~SpheresInstances(){
    delete[] pos;
    delete[] scale;
}

void SpheresInstances::setShaders() {
    compileShaders(R"END(

        #version 330 core
        #extension GL_ARB_explicit_uniform_location : require
        #extension GL_ARB_shading_language_420pack : require

        layout(location = 0) in vec3 vertexPosition_modelspace;
        layout(location = 1) in vec3 offsetPos;
        layout(location = 2) in vec3 offsetScale;
        layout(location = 3) in vec3 normal;

        layout(location = 0)uniform mat4 MVP;
        layout(location = 1)uniform vec3 light;
        layout(location = 2)uniform vec3 light2;

        out vec3 vpos;
        out vec3 vcol;

        void main(){
            float R = length(offsetPos - light);
            vec3 L = (offsetPos - light)/R;
            vec3 L2 = (offsetPos - light2) / length(offsetPos - light2);

            gl_Position =  MVP * (vec4(vertexPosition_modelspace,1) * vec4(offsetScale, 1) + vec4(offsetPos,0));

            float r = R;
            if(r > 1){
                r = 1.0;
            }

            if( acos(dot(normal, L) / length(normal) / length(L)) < 3.141593/2.0){
                vcol = vec3(0,0,0.75) * (length(dot(L, normal)))*(1-r);
            }
            else{
                vcol = vec3(0.75,0,0) * (length(dot(normal,L)))*(1-r);
            }

            if( acos(dot(normal, L2) / length(normal) / length(L2)) > 3.141593/2.0){
                vcol += vec3(0,0.75,0.75) * (length(dot(normal,L2)));
            }
            else{
                vcol += offsetPos * 0.5 * (length(dot(normal,L2)));
            }
            vpos = offsetPos + offsetScale * vertexPosition_modelspace;
        }
    )END", R"END(

        #version 330 core

        in vec3 vpos;
        in vec3 vcol;

        out vec4 color;

        void main(){
            vec3 ocol = vec3(0.5,1,0.5);
            ocol = ocol*((vpos[1]+1) * 0.5);

            // aplha blending
            float I = length(vcol) + length(ocol)*(1.0 - length(vcol));
            ocol = ( vcol*length(vcol) + ocol*length(ocol) * (1.0 - length(vcol)) ) / I;

            ocol = ocol*((vpos[1]+1) * 0.9);

            color = vec4(
                ocol[0],
                ocol[1],
                ocol[2],
                (0.25 + 0.6 * length(vcol))
            );

            //color *= ((vpos[1]+1) * 1);

        }
    )END");
}

void SpheresInstances::setBuffers() {
    bindBuffers();

    GLuint vertexbuffer;   
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * g_vertex_buffer_data.size(), g_vertex_buffer_data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );



    //GLuint normalbuffer;   
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * g_normal_buffer_data.size(), g_normal_buffer_data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        3,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    //unsigned int rotationVBO;
    //glGenBuffers(1, &rotationVBO);
    //glBindBuffer(GL_ARRAY_BUFFER, rotationVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sizeInstances, &rotations[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
//
    //glEnableVertexAttribArray(3);
    //glBindBuffer(GL_ARRAY_BUFFER, rotationVBO);
    //glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE, sizeof(GLfloat)*3, (void*)0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //
    //glVertexAttribDivisor(3,1);


    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    //glDisableVertexAttribArray(3);
}

void SpheresInstances::draw(glm::mat4& MVP,const glm::vec3& light) {
    bindProgram();
    bindBuffers();

    unsigned int posVBO;
    glGenBuffers(1, &posVBO);
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sizeInstances, &pos[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, sizeof(GLfloat)*3, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glVertexAttribDivisor(1,1);


    unsigned int sizeVBO;
    glGenBuffers(1, &sizeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sizeInstances, &scale[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE, sizeof(GLfloat)*3, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glVertexAttribDivisor(2,1);


    glUniformMatrix4fv(0, 1, GL_FALSE, &MVP[0][0]);
    glUniform3f(1, light[0], light[1], light[2]);
    glUniform3f(2, 0.0f, 1.0f, 2.0f);

    glDrawArraysInstanced(GL_TRIANGLES, 0, g_vertex_buffer_data.size(), sizeInstances); // 12*3 indices starting at 0 -> 12 triangles

}

void SpheresInstances::setColor(float r, float g, float b){
    cross_color[0]=r;cross_color[1]=g;cross_color[2]=b;
}