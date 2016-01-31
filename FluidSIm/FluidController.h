#pragma once

#include "GameController.h"

#include "Cube.h"

#include <glm/gtc/random.hpp>
#include <vector>

/*
* Sets up and showcases several different springs
*/
class FluidController : public GameController
{

public:

	FluidController();
	~FluidController();

	// GameController Interface
	virtual void Initialize() override;
	virtual void Update(const GLdouble& dt) override;
	virtual void ProcessInput(const GLint& Key, const GLint& Action, const GLint& Mode) override;
	virtual void Render() override;

private:

	// Structure to contain blob data
	struct Blob
	{
		Blob(const glm::vec3& GridPos) : Radius(glm::linearRand(0.5f, 1.f)), Position(GridPos)
		{
			Velocity = glm::vec3(glm::linearRand(-0.5f, 0.5f), glm::linearRand(-0.5f, 0.5f), glm::linearRand(-0.5f, 0.5f));
			//Velocity = glm::vec3(.25f, 0.f, 0.f);
		};
		GLfloat Radius;
		glm::vec3 Position, Velocity;
	};

	GLfloat ComputeVoxel(const GLuint& gx, const GLuint& gy, const GLuint& gz) const;

	GLfloat GridToWorldX(const GLuint& gx) const;
	GLfloat GridToWorldY(const GLuint& gy) const;
	GLfloat GridToWorldZ(const GLuint& gz) const;

	GLuint WorldToGridX(const GLfloat& gx) const;
	GLuint WorldToGridY(const GLfloat& gy) const;
	GLuint WorldToGridZ(const GLfloat& gz) const;

	const GLuint GridSize = 48;
	std::vector<std::vector<std::vector<GLfloat>>> Grid;
	std::vector<Blob> Blobs;
	GLfloat Scale;
	Cube* Voxel;
};

