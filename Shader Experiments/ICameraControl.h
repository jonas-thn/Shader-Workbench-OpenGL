#pragma once

class IApplicationContext
{
public:
    virtual ~IApplicationContext() = default;

    virtual void SetCameraConfig(float radius, float speed, float height) = 0;

    virtual void ToggleWindowLayout() = 0;
};