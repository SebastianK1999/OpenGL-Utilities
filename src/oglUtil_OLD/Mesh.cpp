#include "oglUtil_OLD/Mesh.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <math.h>
#include "whereami.h"

static constexpr bool LOG_RESOURCE_LOADING = 0;

Mesh::Mesh(const std::string& _path)
: Drawable()
, DrawableBase()
, meshPath("")
, texturePath("")
, vsPath("")
, fsPath("")
, primaryColor(1,1,1)
{
	int dirname_length;
    int length = wai_getExecutablePath(NULL, 0, NULL);
    std::string exePath(length, '\0');
    wai_getExecutablePath(&exePath[0], length, &dirname_length);
    std::string dirPath(exePath.c_str(), static_cast<std::size_t>(dirname_length)+1);

	meshPath = dirPath + _path;
	texturePath = dirPath + _path;
	vsPath = dirPath + "../res/shaders/mesh/";
	fsPath = dirPath + "../res/shaders/mesh/";

    if(meshPath.back() != '/'){
        meshPath.append("/");
        texturePath.append("/");
        vsPath.append("/");
        fsPath.append("/");
    }
    meshPath.append("model.obj");
    texturePath.append("texture.bmp");
    vsPath.append("shader.vs");
    fsPath.append("shader.fs");

    texture = loadBMP(texturePath.c_str());
    loadOBJ(meshPath.c_str(), vertex_buffer_data, uvs_buffer_data, normal_buffer_data);

    shader = LoadShaders(vsPath.c_str(), fsPath.c_str());
    //setShaders();
    setBuffers();
}

Mesh::Mesh(const Mesh& other) noexcept
: Drawable(other)
, DrawableBase(other)
, meshPath(other.meshPath)
, texturePath(other.texturePath)
, vsPath(other.vsPath)
, fsPath(other.fsPath)
, primaryColor(other.primaryColor)
, vertex_buffer_data(other.vertex_buffer_data)
, uvs_buffer_data(other.uvs_buffer_data)
, normal_buffer_data(other.normal_buffer_data)
, texture(other.texture)
, shader(other.shader)
{   
    setBuffers();
}

Mesh::Mesh(Mesh&& other) noexcept
: Drawable(std::move(other))
, DrawableBase(std::move(other))
, meshPath(std::move(other.meshPath))
, texturePath(std::move(other.texturePath))
, vsPath(std::move(other.vsPath))
, fsPath(std::move(other.fsPath))
, primaryColor(std::move(other.primaryColor))
, vertex_buffer_data(std::move(other.vertex_buffer_data))
, uvs_buffer_data(std::move(other.uvs_buffer_data))
, normal_buffer_data(std::move(other.normal_buffer_data))
, texture(std::move(other.texture))
, shader(std::move(other.shader))
{   
    setBuffers();
}

Mesh& Mesh::operator=(const Mesh& other) noexcept
{   
	meshPath = other.meshPath;
	texturePath = other.texturePath;
	vsPath = other.vsPath;
	fsPath = other.fsPath;
	primaryColor = other.primaryColor;
	vertex_buffer_data = other.vertex_buffer_data;
	uvs_buffer_data = other.uvs_buffer_data;
	normal_buffer_data = other.normal_buffer_data;
	texture = other.texture;
	shader = other.shader;
    setBuffers();
	return *this;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{   
	meshPath = std::move(other.meshPath);
	texturePath = std::move(other.texturePath);
	vsPath = std::move(other.vsPath);
	fsPath = std::move(other.fsPath);
	primaryColor = std::move(other.primaryColor);
	vertex_buffer_data = std::move(other.vertex_buffer_data);
	uvs_buffer_data = std::move(other.uvs_buffer_data);
	normal_buffer_data = std::move(other.normal_buffer_data);
	texture = std::move(other.texture);
	shader = std::move(other.shader);
	setBuffers();
	return *this;
}

bool Mesh::loadOBJ(const char * path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals){
    	if constexpr (LOG_RESOURCE_LOADING) printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if( file == NULL ){
		if constexpr (LOG_RESOURCE_LOADING) printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
            //std::cout << temp_vertices.back().x << " " << temp_vertices.back().y << " " << temp_vertices.back().z << "\n";
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			//uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
            //std::cout << temp_uvs.back().x << " " << temp_uvs.back().y << "\n";
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
            //std::cout << temp_normals.back().x << " " << temp_normals.back().y << " " << temp_normals.back().z << "\n";
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3] );
			if (matches == 12){
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
            else{
                //matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if(matches == 9){
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
                else{
                    if constexpr (LOG_RESOURCE_LOADING) printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                    fclose(file);
                    return false;
                }
            } 
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
        
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	}
	fclose(file);
	return true;
}

GLuint Mesh::loadBMP(const char * imagePath){

	if constexpr (LOG_RESOURCE_LOADING) printf("Reading image %s\n", imagePath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagePath,"rb");
	if (!file){
		if constexpr (LOG_RESOURCE_LOADING) printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagePath);
		getchar();
		return 0;
	}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if ( fread(header, 1, 54, file)!=54 ){ 
		if constexpr (LOG_RESOURCE_LOADING) printf("Not a correct BMP file\n");
		fclose(file);
		return 0;
	}
	// A BMP files always begins with "BM"
	if ( header[0]!='B' || header[1]!='M' ){
		if constexpr (LOG_RESOURCE_LOADING) printf("Not a correct BMP file\n");
		fclose(file);
		return 0;
	}
	// Make sure this is a 24bpp file
	if ( *(int*)&(header[0x1E])!=0  )         {if constexpr (LOG_RESOURCE_LOADING) printf("Not a correct BMP file\n");    fclose(file); return 0;}
	if ( *(int*)&(header[0x1C])!=24 )         {if constexpr (LOG_RESOURCE_LOADING) printf("Not a correct BMP file\n");    fclose(file); return 0;}

	// Read the information about the image
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);


	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
    fseek(file, (long)dataPos, SEEK_SET);
	fread(data,1,imageSize,file);

	// Everything is in memory now, the file can be closed.
	fclose (file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );

	// Give the image to OpenGL
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version
	delete [] data;

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// ... nice trilinear filtering ...
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
	// ... which requires mipmaps. Generate them automatically.
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}

GLuint Mesh::LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		if constexpr (LOG_RESOURCE_LOADING) printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}else{
		if constexpr (LOG_RESOURCE_LOADING) printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", fragment_file_path);
		getchar();
		return 0;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	if constexpr (LOG_RESOURCE_LOADING) printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		if constexpr (LOG_RESOURCE_LOADING) printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	if constexpr (LOG_RESOURCE_LOADING) printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		if constexpr (LOG_RESOURCE_LOADING) printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	if constexpr (LOG_RESOURCE_LOADING) printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		if constexpr (LOG_RESOURCE_LOADING) printf("%s\n", &ProgramErrorMessage[0]);
	}

	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void Mesh::setShader(const std::string& _path){
	int dirname_length;
    int length = wai_getExecutablePath(NULL, 0, NULL);
    std::string exePath(length, '\0');
    wai_getExecutablePath(&exePath[0], length, &dirname_length);
    std::string dirPath(exePath.c_str(), static_cast<std::size_t>(dirname_length)+1);

    vsPath = dirPath + _path;
    fsPath = dirPath + _path;
    if(vsPath.back() != '/'){
        vsPath.append("/");
        fsPath.append("/");
    }
    vsPath.append("shader.vs");
    fsPath.append("shader.fs");

    shader = LoadShaders(vsPath.c_str(), fsPath.c_str());
}

void Mesh::setBuffers() {
    bindBuffers();

    GLuint vertexBuffer;    
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertex_buffer_data.size(), vertex_buffer_data.data(), GL_STATIC_DRAW);


    // Set our "myTextureSampler" sampler to use Texture Unit 0
    //glUniform1i(TextureID, 0);


    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    GLuint uvsBuffer;   
    glGenBuffers(1, &uvsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs_buffer_data.size(), uvs_buffer_data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvsBuffer);
    glVertexAttribPointer(
        1,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        2,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    GLuint normalBuffer;   
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normal_buffer_data.size(), normal_buffer_data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(
        2,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
}

void Mesh::draw(const glm::mat4& MVP, const glm::vec3& light) const {
    glBindTexture(GL_TEXTURE_2D, texture);
    glUseProgram(shader);
    bindBuffers();
    glUniformMatrix4fv(0, 1, GL_FALSE, &MVP[0][0]);
    glUniform4f(1, position[0],position[1],position[2],0);
    glUniform4f(2, scale[0],scale[1],scale[2],1);
    glUniform4f(3, rotation[0],rotation[1],rotation[2],1);
    glUniform3f(4, light.x, light.y, light.z);
    glUniform3f(5, primaryColor.x, primaryColor.y, primaryColor.z);
    glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size());
}

void Mesh::reinitialize(){
	glGenVertexArrays(1, &(vaoId));
	bindProgram();
	setShader("../res/shaders/mesh/");
	setBuffers();
	loadBMP(texturePath.c_str());
}

