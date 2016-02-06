#pragma once

#include "GameController.h"
#include "MarchingCubes.h"
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
	virtual void ProcessMouseMove(const GLdouble& dX, const GLdouble& dY) override;
	virtual void Render() override;

private:

	// Structure to contain blob data
	struct Blob
	{
		Blob(const glm::vec3& GridPos) : Radius(glm::linearRand(5.f, 10.f)), Position(GridPos)
		{
			Velocity = glm::vec3(glm::linearRand(-5.f, 10.f), glm::linearRand(-5.f, 10.f), glm::linearRand(-5.f, 10.f));
		};
		GLfloat Radius;
		glm::vec3 Position, Velocity;
	};

	// Computes the charge strength at each griod location
	GLfloat ComputeVoxel(const GLuint& gx, const GLuint& gy, const GLuint& gz) const;

	// State
	const GLuint GridSize = 42;
	std::vector<std::vector<std::vector<GLfloat>>> Grid;
	std::vector<Blob> Blobs;

	// Functions to initialize various aspects of the scene
	void InitSkybox();
	void InitBlobs();

	MarchingCubes Mesh; // Computes the mesh, retrieve vertices, indices and normals from here
	std::map<std::string, GLuint> Buffers; // Buffer storage
};

