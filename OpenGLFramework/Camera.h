#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void SetPosition(const glm::vec3& Position);
	void SetFocus(const glm::vec3& Look);
	void SetWorldUp(const glm::vec3& Up);

	void RotateByMouse(const GLfloat& dX, const GLfloat& dY);

	void UpdateView();

	glm::vec3 GetPosition() const;

private:

	glm::vec3 Position, WorldUp, LocalUp, View;
};

