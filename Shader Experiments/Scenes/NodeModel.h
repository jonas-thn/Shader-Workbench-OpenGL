#pragma once
#include "../Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum NodeType
{
    Master,
    Time,
    Color,
    Random,
    Add,
    Subtract,
    Multiply,
    Value,
    Sin,
    Lerp,
    Min,
    Max,
    Abs,
    Mod
};

inline constexpr const char* nodeTitles[] = 
{
    "Master", "Time", "Color", "Random", "Add", "Subtract", "Multiply", "Value", "Sin", "Lerp", "Min", "Max", "Abs", "Mod"
};

struct MenuEntry 
{
    NodeType type;
    const char* label;
};

inline constexpr MenuEntry inputNodes[] = 
{
    {Value, "Value Node"},
    {Time, "Time Node"},
    {Color, "Color Node"},
	{Random, "Random Node"}
};

inline constexpr MenuEntry mathNodes[] =
{
    {Add, "Add Node"},
    {Subtract, "Subtract Node"},
    {Multiply, "Multiply Node"},
    {Lerp, "Lerp Node"},
    {Min, "Min Node"},
    {Max, "Max Node"},
    {Sin, "Sin Node"},
    {Abs, "Abs Node"},
    {Mod, "Mod Node"}
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