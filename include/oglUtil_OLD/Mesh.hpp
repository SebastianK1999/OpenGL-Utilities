#ifndef MESH_INCLUDED
#define MESH_INCLUDED

#include "oglUtil/drawables/Drawable.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <string>

class Mesh : public oglu::Drawable {
public:
    std::string meshPath;
    std::string texturePath;
    std::string vsPath;
    std::string fsPath;
    glm::vec3 primaryColor;

    Mesh(const std::string& _path);
    void loadFragmentShader(const std::string& _path);
    //~Mesh();
    void setBuffers();
    void setShader(const std::string& _path);
    void draw(const glm::mat4& MVP, const glm::vec3& light);
 private:
    GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
    bool loadOBJ(
        const char * path, 
        std::vector<glm::vec3> & out_vertices, 
        std::vector<glm::vec2> & out_uvs, 
        std::vector<glm::vec3> & out_normals
    );
    GLuint loadBMP(const char * imagePath);

    std::vector<glm::vec3> vertex_buffer_data;
    std::vector<glm::vec2> uvs_buffer_data;
    std::vector<glm::vec3> normal_buffer_data;
    
    GLuint texture;
    GLuint shader;
};

#endif