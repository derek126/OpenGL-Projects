#pragma once

#include "GameController.h"
#include "MarchingCubes.h"

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
		Blob(const glm::vec3& Velocity, const glm::vec3& GridPos, const GLfloat Radius) : Velocity(Velocity), Radius(Radius), Position(GridPos)
		{};

		GLfloat Radius;
		glm::vec3 Position, Velocity;
	};

	// Computes the charge strength at each griod location
	GLfloat ComputeVoxel(const GLuint& gx, const GLuint& gy, const GLuint& gz) const;

	// State
	const GLuint Resolution = 32;
	std::vector<std::vector<std::vector<GLfloat>>> Grid;
	std::vector<Blob> Blobs;

	// Functions to initialize various aspects of the scene
	void InitSkybox();
	void InitBlobs();
	void InitGrass();

	MarchingCubes Mesh; // Computes the mesh, retrieve vertices, indices and normals from here
	std::map<std::string, GLuint> Buffers; // Buffer storage
};

