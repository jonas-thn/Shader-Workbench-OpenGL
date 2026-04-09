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
    Mutliply,
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

    float value = 0.0f;
	glm::vec3 color = glm::vec3(0.0f);
};