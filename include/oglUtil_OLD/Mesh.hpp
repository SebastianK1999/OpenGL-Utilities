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
    Mesh(const Mesh& other) noexcept;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(const Mesh& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
    void loadFragmentShader(const std::string& _path=0);
    //~Mesh();
    void setBuffers();
    void setShader(const std::string& _path);
    void draw(const glm::mat4& MVP, const glm::vec3& light) const;
    void reinitialize();
 private:
    std::vector<glm::vec3> vertex_buffer_data;
    std::vector<glm::vec2> uvs_buffer_data;
    std::vector<glm::vec3> normal_buffer_data;
    GLuint shader;
    GLuint texture;

    GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
    bool loadOBJ(
        const char * path, 
        std::vector<glm::vec3> & out_vertices, 
        std::vector<glm::vec2> & out_uvs, 
        std::vector<glm::vec3> & out_normals
    );
    GLuint loadBMP(const char * imagePath);

};

#endif