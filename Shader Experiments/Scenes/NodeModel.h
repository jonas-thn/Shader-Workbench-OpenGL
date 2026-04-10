#pragma once
#include "../Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum NodeType
{
    Master,
    Time,
    Color,
    Add,
    Subtract,
    Multiply,
    Value,
    Sin
};

struct Link 
{
    int id;
    int startPin;
    int endPin;
};

struct Node 
{
    int id;
    NodeType type;
    std::vector<int> inputPins;
    std::vector<int> outputPins;

	glm::vec3 data = glm::vec3(0.0f);
    float value = 0.0f;
};