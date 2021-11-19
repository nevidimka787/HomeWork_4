#pragma once

#include "Math.h"
#include "../Constants/AllConstants.h"

class Camera
{
public:
    Vec2F position;
    float size;
    float scale;
    
    Camera(const Camera& camera);
    Camera(
        Vec2F position = CAMERA_DEFAULT_POSITION,
        float size = CAMERA_DEFAULT_SIZE);
    
    void Move(Vec2F move_vector);
    
    ~Camera();
};

