#include "Camera.h"

#include "ResourceManager.h"

Camera::Camera() :
	Position(0.f),
	View(0.f),
	WorldUp(0.f, 1.f, 0.f)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(const glm::vec3& Position)
{
	this->Position = Position;
}

void Camera::SetFocus(const glm::vec3& Look)
{
	View = glm::normalize(Look - Position);
}

void Camera::SetWorldUp(const glm::vec3& Up)
{
	WorldUp = glm::normalize(Up);
}

void Camera::RotateByMouse(const GLfloat& dX, const GLfloat& dY)
{
	glm::quat RotatorX;
	glm::quat RotatorY;

	glm::quat qView;
	qView.x = View.x;
	qView.y = View.y;
	qView.z = View.z;
	qView.w = 0.f;

	if (dX != 0)
	{
		RotatorX.x = LocalUp.x * glm::sin(-dX / 2.f);
		RotatorX.y = LocalUp.y * glm::sin(-dX / 2.f);
		RotatorX.z = LocalUp.z * glm::sin(-dX / 2.f);
		RotatorX.w = glm::cos(-dX / 2.f);

		qView = RotatorX * qView * glm::conjugate(RotatorX);
	}

	if (dY != 0)
	{
		glm::vec3 Right = glm::normalize(glm::cross(View, WorldUp));
		RotatorY.x = Right.x * glm::sin(-dY / 2.f);
		RotatorY.y = Right.y * glm::sin(-dY / 2.f);
		RotatorY.z = Right.z * glm::sin(-dY / 2.f);
		RotatorY.w = glm::cos(-dY / 2.f);

		qView = RotatorY * qView * glm::conjugate(RotatorY);
	}

	qView = glm::normalize(qView);
	View.x = qView.x;
	View.y = qView.y;
	View.z = qView.z;
}

void Camera::UpdateView()
{
	glm::vec3 Right = glm::normalize(glm::cross(View, WorldUp));
	LocalUp = glm::normalize(glm::cross(Right, View));

	RESOURCEMANAGER.SetViewMatrix(glm::lookAt(Position, Position + View, LocalUp));
}
