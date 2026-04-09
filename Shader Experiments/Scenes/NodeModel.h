#pragma once

struct Link {
    int id;
    int startPin;
    int endPin;
};

struct Node {
    int id;
    int inputPin;
    int outputPin;
    float value; 
};