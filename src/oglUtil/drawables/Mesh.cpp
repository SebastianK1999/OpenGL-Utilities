/*
* MIT License
* 
* Copyright (c) 20233 Sebastian Kwaśniak
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


#include <utility>
#include <cstring>
#include <array>
#include <iostream>

#include "whereami.h"

#include "oglUtil/drawableSystem/DrawableRegistry.hpp"
#include "oglUtil/drawableSystem/ShaderCollection.hpp"
#include "oglUtil/tools/OgluFunctions.hpp"

#include "oglUtil/drawables/Mesh.hpp"

static constexpr bool LOG_RESOURCE_LOADING = 0;

std::string oglu::Mesh::modelFileName = "model.obj";

std::string oglu::Mesh::textureFileName = "texture.bmp";

std::shared_ptr<oglu::RegisteredDrawable> oglu::Mesh::registerMesh(const std::filesystem::path& meshDirectory)
{
    const std::string key = oglu::Mesh::createKey(meshDirectory);
    if(!oglu::DrawableRegistry::hasRegistry(key))
    {
        std::vector<glm::vec3> vertexBuffer;
        std::vector<glm::vec3> normalBuffer;
        std::vector<glm::vec2> uvBuffer;
        // TODO isSuccess bool;
        oglu::Mesh::loadObject(meshDirectory, vertexBuffer, normalBuffer, uvBuffer);
        unsigned int textureWidth;
        unsigned int textureHeight;
        std::vector<unsigned char> textureData;
        // TODO isSuccess bool;
        oglu::Mesh::loadBMP(meshDirectory, textureData, textureWidth, textureHeight);
        return oglu::DrawableRegistry::registerDrawable
        (
            key,
            vertexBuffer,
            {},
            normalBuffer,
            uvBuffer,
            textureData,
            textureWidth,
            textureHeight,
            oglu::ShaderCollection::texturedVertexShader,
            oglu::ShaderCollection::texturedFragmentShader
        );
    }
    return oglu::DrawableRegistry::getRegistry(key);
}

oglu::RegisteredDrawable& oglu::Mesh::getRegistry(const std::filesystem::path& meshDirectory)
{
    return *(oglu::DrawableRegistry::getRegistry(oglu::Mesh::createKey(meshDirectory)));
}

oglu::RegisteredDrawable& oglu::Mesh::getRegistry(const std::string& key)
{
    return *(oglu::DrawableRegistry::getRegistry(key));
}

void oglu::Mesh::drawInstances(const std::filesystem::path& meshDirectory, const glm::mat4& MVP, const glm::vec3& light)
{
    if(!oglu::DrawableRegistry::hasRegistry(oglu::Mesh::createKey(meshDirectory)))
    {
        return;
    }
    oglu::DrawableRegistry::getRegistry(oglu::Mesh::createKey(meshDirectory))->drawInstances(MVP, light);
}

void oglu::Mesh::drawInstances(const std::string& key, const glm::mat4& MVP, const glm::vec3& light)
{
    oglu::DrawableRegistry::getRegistry(key)->drawInstances(MVP, light);
}

oglu::RegisteredDrawable& oglu::Mesh::getRegistry() const noexcept 
{
    return *(registryPointer);
}

oglu::Mesh::Mesh(const std::filesystem::path& meshDirectory)
{
    registryPointer = oglu::Mesh::registerMesh(meshDirectory);
    instancePointer = registryPointer->addInstance();
}

oglu::Mesh::Mesh(oglu::RegisteredDrawable& meshTypeRegistry)
{
    registryPointer = std::shared_ptr<oglu::RegisteredDrawable>(&meshTypeRegistry);
    instancePointer = registryPointer->addInstance();
}

const std::string oglu::Mesh::createKey(const std::filesystem::path& meshDirectory)noexcept
{
    return std::string() + "_oglu::Mesh(path=\"" + meshDirectory.string() + "\")";
}

const bool oglu::Mesh::loadObject
(
    const std::filesystem::path& meshDirectory,
    std::vector<glm::vec3>& vertexBuffer,
    std::vector<glm::vec3>& normalBuffer,
    std::vector<glm::vec2>& uvBuffer
)
{
	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> uvIndices;
	std::vector<unsigned int> normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec2> temp_uvs;
    int dirnameLength;
    int executablePathLength = wai_getExecutablePath(NULL, 0, NULL);
    std::string exePath(executablePathLength, '\0');
    wai_getExecutablePath(&exePath[0], executablePathLength, &dirnameLength);
    std::string dirPath(exePath.c_str(), static_cast<std::size_t>(dirnameLength)+1);
    std::filesystem::path meshPath = std::filesystem::path(dirPath) / meshDirectory / oglu::Mesh::modelFileName;
	FILE * file = fopen(meshPath.c_str(), "r");
	if( file == NULL )
    {
		if constexpr (LOG_RESOURCE_LOADING) oglu::printErrorsDbg(std::string("Impossible to open the file: ") + meshPath.string());
		return false;
	}
    // TODO - scan twice to count vertices and uvs
    // TODO - separate loops for v, vt, vn, f ? (if possible)
	while(true)
    {
		char lineHeader[128];
		int lineType = fscanf(file, "%s", lineHeader);
		if (lineType == EOF)
        {
			break;
        }
		if(strcmp( lineHeader, "v" ) == 0)
        {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
        else if ( strcmp( lineHeader, "vt" ) == 0 )
        {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			//uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
        else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
        }
        else if ( strcmp( lineHeader, "f" ) == 0 )
        {
			unsigned int vertexIndex[4];
			unsigned int normalIndex[4];
			unsigned int uvIndex[4];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3] );
			if (matches == 12)
            {
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[2]);
                vertexIndices.push_back(vertexIndex[3]);
                uvIndices    .push_back(uvIndex[0]);
                uvIndices    .push_back(uvIndex[1]);
                uvIndices    .push_back(uvIndex[2]);
                uvIndices    .push_back(uvIndex[0]);
                uvIndices    .push_back(uvIndex[2]);
                uvIndices    .push_back(uvIndex[3]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[2]);
                normalIndices.push_back(normalIndex[3]);
			}
            else
            {
                //matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if(matches == 9)
                {
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    uvIndices    .push_back(uvIndex[0]);
                    uvIndices    .push_back(uvIndex[1]);
                    uvIndices    .push_back(uvIndex[2]);
                    normalIndices.push_back(normalIndex[0]);
                    normalIndices.push_back(normalIndex[1]);
                    normalIndices.push_back(normalIndex[2]);
                }
                else
                {
                    if constexpr (LOG_RESOURCE_LOADING) oglu::printErrorsDbg("File can't be read because this parser sucks ☹️. Try implementing/including new one\n");
                    fclose(file);
                    return false;
                }
            } 
		}
        else
        {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}
    vertexBuffer.reserve(vertexIndices.size());
    normalBuffer.reserve(vertexIndices.size());
    uvBuffer.reserve(vertexIndices.size());
	for( unsigned int i=0; i<vertexIndices.size(); i++ )
    {   
		vertexBuffer.emplace_back(temp_vertices[vertexIndices[i]-1]);
		normalBuffer.emplace_back(temp_normals[normalIndices[i]-1]);
		uvBuffer.emplace_back(temp_uvs[uvIndices[i]-1]);
	}
	fclose(file);
	return true;
}

const bool oglu::Mesh::loadBMP
(
    const std::filesystem::path& meshDirectory,
    std::vector<unsigned char>& textureData,
    unsigned int& width,
    unsigned int& height
)
{
    if constexpr (LOG_RESOURCE_LOADING) printf("Reading image %s\n", meshDirectory.c_str());
    std::array<unsigned char, 54> header;
	unsigned int dataOffset;
	unsigned int imageSize;
    int dirnameLength;
    int executablePathLength = wai_getExecutablePath(NULL, 0, NULL);
    std::string exePath(executablePathLength, '\0');
    wai_getExecutablePath(&exePath[0], executablePathLength, &dirnameLength);
    std::string dirPath(exePath.c_str(), static_cast<std::size_t>(dirnameLength)+1);
    std::filesystem::path imagePath = std::filesystem::path(dirPath) / meshDirectory / oglu::Mesh::textureFileName;
	FILE * file = fopen(imagePath.c_str(),"rb");
	if (!file){
		if constexpr (LOG_RESOURCE_LOADING) printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagePath.c_str());
		getchar();
		return false;
	}
	// If less than 54 bytes are read, problem
	if ( fread(header.data(), 1, 54, file)!=54 )
    { 
		if constexpr (LOG_RESOURCE_LOADING) printf("Not a correct BMP file\n");
		fclose(file);
		return false;
	}
	// A BMP files always begins with "BM"
	if ( header[0]!='B' || header[1]!='M' )
    {
		if constexpr (LOG_RESOURCE_LOADING) printf("Not a correct BMP file\n");
		fclose(file);
		return false;
	}
	// Make sure this is a 24bpp file
	if ( *(int*)&(header[0x1E])!=0  )
    {
        if constexpr (LOG_RESOURCE_LOADING) printf("Not a correct BMP file\n");
        fclose(file);
        return false;
    }
	if ( *(int*)&(header[0x1C])!=24 )
    {
        if constexpr (LOG_RESOURCE_LOADING) printf("Not a correct BMP file\n");
        fclose(file);
        return false;
    }

	// Read the information about the image
	dataOffset  = *(int*)&(header[0x0A]);
	imageSize   = *(int*)&(header[0x22]);
	width       = *(int*)&(header[0x12]);
	height      = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataOffset==0)      dataOffset=54; // The BMP header is done that way

	// Create a buffer
    textureData.resize(imageSize);

	// Read the actual data from the file into the buffer
    fseek(file, (long)dataOffset, SEEK_SET);
	fread(textureData.data(),1,imageSize,file);

	// Everything is in memory now, the file can be closed.
	fclose (file);
    return true;
}