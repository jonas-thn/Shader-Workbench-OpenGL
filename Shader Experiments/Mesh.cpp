#include "Mesh.h"

Mesh::Mesh(const std::string& path) : path(path)
{
    LoadMesh(); 
    SetupMesh(); 
}

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) : vertices(vertices), indices(indices)
{
    SetupMesh(); 
}

Mesh::~Mesh()
{
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (EBO != 0) glDeleteBuffers(1, &EBO);
}

Mesh::Mesh(Mesh&& other) noexcept : vertices(std::move(other.vertices)),
    indices(std::move(other.indices)), VBO(other.VBO), VAO(other.VAO),
    EBO(other.EBO), path(std::move(other.path)), model(other.model)
{
    other.VBO = 0;
    other.VAO = 0;
    other.EBO = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other)
    {
        if (VAO != 0) glDeleteVertexArrays(1, &VAO);
        if (VBO != 0) glDeleteBuffers(1, &VBO);
        if (EBO != 0) glDeleteBuffers(1, &EBO);

        vertices = std::move(other.vertices);
        indices = std::move(other.indices);
        path = std::move(other.path);
        model = other.model;

        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;

        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
    }
    return *this;
}

void Mesh::SetupMesh()
{
    if (vertices.empty())
    {
        return;
    }

    const int stride = 8 * sizeof(float);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    if (!indices.empty())
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // uv
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::LoadMesh()
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "Error Assimp: " << import.GetErrorString() << std::endl;
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}

void Mesh::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

void Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        if (mesh->HasNormals())
        {
            vertices.push_back(mesh->mNormals[i].x);
            vertices.push_back(mesh->mNormals[i].y);
            vertices.push_back(mesh->mNormals[i].z);
        }
        else
        {
            vertices.insert(vertices.end(), { 0.0f, 0.0f, 0.0f });
        }

        if (mesh->mTextureCoords[0])
        {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertices.insert(vertices.end(), { 0.0f, 0.0f });
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
}

void Mesh::Translate(glm::vec3 pos)
{
    model = glm::translate(model, pos);
}

void Mesh::SetPos(glm::vec3 pos)
{
    model = glm::translate(glm::mat4(1.0f), pos);
}

void Mesh::Rotate(float angle, const glm::vec3& axis)
{
    model = glm::rotate(model, glm::radians(angle), axis);
}

void Mesh::RotateWorld(float dx, float dy, float sensitivity)
{
    glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), glm::radians(dx * sensitivity), glm::vec3(0, 1, 0));
    glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f), glm::radians(dy * sensitivity), glm::vec3(1, 0, 0));
    model = yawRotation * model;
    model = pitchRotation * model;
}

void Mesh::ResetScale()
{
    glm::vec3 scale;
    scale.x = glm::length(glm::vec3(model[0]));
    scale.y = glm::length(glm::vec3(model[1]));
    scale.z = glm::length(glm::vec3(model[2]));

    glm::mat4 result = model;
    result[0] = glm::vec4(glm::normalize(glm::vec3(model[0])), 0.0f);
    result[1] = glm::vec4(glm::normalize(glm::vec3(model[1])), 0.0f);
    result[2] = glm::vec4(glm::normalize(glm::vec3(model[2])), 0.0f);
    result[3] = model[3];
    model = result;
}

void Mesh::SetScale(glm::vec3 scale)
{
    glm::vec3 xAxis = glm::normalize(glm::vec3(model[0]));
    glm::vec3 yAxis = glm::normalize(glm::vec3(model[1]));
    glm::vec3 zAxis = glm::normalize(glm::vec3(model[2]));
    glm::vec3 translation = glm::vec3(model[3]);

    xAxis *= scale.x;
    yAxis *= scale.y;
    zAxis *= scale.z;

    model[0] = glm::vec4(xAxis, 0.0f);
    model[1] = glm::vec4(yAxis, 0.0f);
    model[2] = glm::vec4(zAxis, 0.0f);
    model[3] = glm::vec4(translation, 1.0f);
}

void Mesh::Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection,
    const glm::vec3& camPos, float time, const glm::vec2& resolution, int index, int count) const
{
    if (vertices.empty()) return;

    shader.Use();
    shader.SetMat4("model", model);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);
    shader.SetVec3("cameraPos", camPos);
    shader.SetFloat("time", time);
    shader.SetInt("index", index);
    shader.SetVec2("resolution", resolution);

    glBindVertexArray(VAO);

    if (count <= 0)
    {
        if (indices.empty())
        {
            glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 8);
        }
        else
        {
            glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
        }
    }
    else
    {
        if (indices.empty())
        {
            glDrawArraysInstanced(GL_TRIANGLES, 0, vertices.size() / 8, count);
        }
        else
        {
            glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0, count);
        }
    }

    glBindVertexArray(0);
}