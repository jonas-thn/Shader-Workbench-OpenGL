#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"

class Mesh
{
public:
    Mesh(const std::string& path);
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void Translate(glm::vec3 pos);
    void SetPos(glm::vec3 pos);
    void Rotate(float angle, const glm::vec3& axis);
    void RotateWorld(float dx, float dy, float sensitivity);
    void ResetScale();
    void SetScale(glm::vec3 scale);

    std::vector<float>& GetVertices() { return vertices; }
    std::vector<unsigned int>& GetIndices() { return indices; }

    void Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& camPos,
        float time, const glm::vec2& resolution, int index, int count, const glm::vec3& nodeColor = glm::vec3(-1.0f)) const;

private:
    void LoadMesh();
    void ProcessNode(aiNode* node, const aiScene* scene);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene);

    void SetupMesh();

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int VBO = 0;
    unsigned int VAO = 0;
    unsigned int EBO = 0;

    std::string path = "";
    glm::mat4 model = glm::mat4(1.0f);
};