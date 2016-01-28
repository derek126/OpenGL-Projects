#include "Camera.h"

#include "ResourceManager.h"

Camera::Camera() :
	Position(0.f),
	LookAt(0.f),
	WorldUp(0.f, 1.f, 0.f)
{
}

Camera::~Camera()
{
}

void Camera::Translate(const glm::vec3& Translation)
{
	Position += Translation;
	LookAt += Translation;
}

void Camera::SetPosition(const glm::vec3& Translation)
{
	Position = Translation;
}

void Camera::SetLookAt(const glm::vec3& Look)
{
	LookAt = Look;
}

void Camera::SetWorldUp(const glm::vec3& Up)
{
	WorldUp = glm::normalize(Up);
}

void Camera::UpdateView()
{
	RESOURCEMANAGER.SetViewMatrix(glm::lookAt(Position, LookAt, WorldUp));
}
