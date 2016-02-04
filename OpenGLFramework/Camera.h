#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void SetPosition(const glm::vec3& Translation);
	void SetFocus(const glm::vec3& Look);
	void SetWorldUp(const glm::vec3& Up);

	void Translate(const glm::vec3& Translation);

	void UpdateView();

private:

	glm::vec3 Position, Focus, WorldUp;

};

