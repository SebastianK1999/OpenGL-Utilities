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


#include "oglUtil/drawableSystem/ShaderLayoutEnums.hpp"

#include "oglUtil/drawableSystem/ShaderCollection.hpp"

const std::string oglu::ShaderCollection::vertexShaderLayouts = 
std::string()
+ "#version 330 core \n"
+ "#extension GL_ARB_explicit_uniform_location : require \n"
+ "#extension GL_ARB_shading_language_420pack : require \n"

+ "layout(location = "+ std::to_string(oglu::EShaderBufferLayout::vertex)         +") in vec3 vVertex; \n"
+ "layout(location = "+ std::to_string(oglu::EShaderBufferLayout::vertexColor)    +") in vec4 vVertexColor; \n"
+ "layout(location = "+ std::to_string(oglu::EShaderBufferLayout::normal)         +") in vec3 vNormal; \n"
+ "layout(location = "+ std::to_string(oglu::EShaderBufferLayout::uv)             +") in vec2 vUV; \n"

+ "layout(location = "+ std::to_string(oglu::EShaderBufferLayout::position)       +") in vec3 vPosition; \n"
+ "layout(location = "+ std::to_string(oglu::EShaderBufferLayout::scale)          +") in vec3 vScale; \n"
+ "layout(location = "+ std::to_string(oglu::EShaderBufferLayout::rotation)       +") in vec3 vRotation; \n"
+ "layout(location = "+ std::to_string(oglu::EShaderBufferLayout::color)          +") in vec4 vColor; \n"


+ "layout(location = "+ std::to_string(oglu::EShaderUniformLayout::MVP)     +") uniform mat4 vMVP; \n"
+ "layout(location = "+ std::to_string(oglu::EShaderUniformLayout::light)   +") uniform vec3 vLight; \n"
;

const std::string oglu::ShaderCollection::fragmentShaderLayouts = 
std::string()
+ "#version 330 core \n"
;

const std::string oglu::ShaderCollection::vertexShaderFunctions = 
std::string()
+ R"END(
    vec3 rotate(vec3 vertex, vec3 rotations)
    {
        vec3 result;
        float cosX = cos(rotations.x);
        float sinX = sin(rotations.x);
        float cosY = cos(rotations.y);
        float sinY = sin(rotations.y);
        float cosZ = cos(rotations.z);
        float sinZ = sin(rotations.z);
        
        mat3 rotationMatrix = mat3(
            cosY * cosZ,
            -cosX * sinZ + sinX * sinY * cosZ,
            sinX * sinZ + cosX * sinY * cosZ,
            cosY * sinZ,
            cosX * cosZ + sinX * sinY * sinZ,
            -sinX * cosZ + cosX * sinY * sinZ,
            -sinY,
            sinX * cosY,
            cosX * cosY
        );
        
        result = rotationMatrix * vertex;
        
        return result;
    }
)END"
;

const oglu::VertexShader oglu::ShaderCollection::basicVertexShader = 
    vertexShaderLayouts + 
    vertexShaderFunctions +
R"END(
    out vec3 fPosition;
    out vec3 fNormal;
    out vec3 fLight;
    out vec4 fColor;

    void main()
    {	
        vec3 vertex;

        vertex = vVertex * vScale;
        vertex = rotate(vertex, vRotation);
        vertex = vertex + vPosition;

        gl_Position =  vMVP * vec4(vertex,1);
        fPosition = vertex;
        fNormal = rotate(vNormal, vRotation);
        fLight = vLight;
        fColor = vColor;
    }

)END";

const oglu::FragmentShader oglu::ShaderCollection::basicFragmentShader = 
    fragmentShaderLayouts + 
R"END(
    in vec3 fPosition;
    in vec3 fNormal;
    in vec3 fLight;
    in vec4 fColor;

    out vec4 Color;

    void main()
    {
        vec3 light = normalize(fLight - fPosition);
        vec3 normal = normalize(fNormal);
        float shading = clamp(dot(normal,light),0,1);
        Color = fColor * (shading * 0.7 + 0.3);
        Color.a = fColor.a;
    }

)END";

const oglu::VertexShader oglu::ShaderCollection::texturedVertexShader = 
    vertexShaderLayouts +
    vertexShaderFunctions +
R"END(
    out vec3 fPosition;
    out vec3 fNormal;
    out vec3 fLight;
    out vec4 fColor;
    out vec2 fUV;

    void main()
    {	
        vec3 vertex;

        vertex = vVertex * vScale;
        vertex = rotate(vertex, vRotation);
        vertex = vertex + vPosition;

        gl_Position =  vMVP * vec4(vertex,1);
        fPosition = vertex;
        fNormal = rotate(vNormal, vRotation);
        fLight = vLight;
        fColor = vColor;
        fUV = vUV;
    }

)END";

const oglu::FragmentShader oglu::ShaderCollection::texturedFragmentShader =
    fragmentShaderLayouts + 
R"END(
    in vec3 fPosition;
    in vec3 fNormal;
    in vec3 fLight;
    in vec4 fColor;
    in vec2 fUV;

    out vec4 Color;

    uniform sampler2D myTextureSampler;

    void main()
    {
        vec3 light = normalize(fLight - fPosition);
        vec3 normal = normalize(fNormal);
        float shading = clamp(dot(normal,light),0,1);
        Color = texture( myTextureSampler, fUV);
        Color = Color * shading;
        Color.a = fColor.a;
    }

)END";