#include "Camera.h"

#include "ResourceManager.h"

Camera::Camera() :
	Position(0.f),
	Focus(0.f),
	WorldUp(0.f, 1.f, 0.f)
{
}

Camera::~Camera()
{
}

void Camera::Translate(const glm::vec3& Translation)
{
	Position += Translation;
	Focus += Translation;
}

void Camera::SetPosition(const glm::vec3& Translation)
{
	Position = Translation;
}

void Camera::SetFocus(const glm::vec3& Look)
{
	Focus = Look;
}

void Camera::SetWorldUp(const glm::vec3& Up)
{
	WorldUp = glm::normalize(Up);
}

void Camera::UpdateView()
{
	RESOURCEMANAGER.SetViewMatrix(glm::lookAt(Position, Position + glm::normalize(Focus - Position), WorldUp));
}
