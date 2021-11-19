#include "Camera.h"

Camera::Camera(const Camera& camera) :
    position(camera.position),
    size(camera.size)
{
}

Camera::Camera(Vec2F position, float size) :
    position(position),
    size(size)
{
}

void Camera::Move(Vec2F move_vcetor)
{
    position += move_vcetor;
}

Camera::~Camera()
{
}
















