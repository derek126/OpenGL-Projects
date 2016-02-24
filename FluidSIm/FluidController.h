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
		{
			RadiusSquared = glm::pow(Radius, 2);
		};

		GLfloat Radius, RadiusSquared;
		glm::vec3 Position, Velocity;
	};

	// Functions for computing the mesh
	void ComputeVoxels();
	GLfloat ComputeAtGrid(const GLuint& ix, const GLuint& iy, const GLuint& iz);
	void AddNeighbours(const GLuint& gx, const GLuint& gy, const GLuint& gz);
	void ComputeNeighbours(const GLuint& gx, const GLuint& gy, const GLuint& gz);

	// State
	const GLuint Resolution = 64;
	std::vector<std::vector<std::vector<GLfloat>>> Grid;
	std::vector<Blob> Blobs;

	// Buffers to refrain from allocating new memory every frame...
	std::vector<std::vector<std::vector<GLboolean>>> IsComputed;
	std::vector<glm::vec3> Neighbours;

	// Functions to initialize various aspects of the scene
	void InitSkybox();
	void InitBlobs();
	void InitGrass();

	MarchingCubes* MeshBuilder; // Computes the mesh, retrieve vertices, indices and normals from here
	std::map<std::string, GLuint> Buffers; // Buffer storage
};

