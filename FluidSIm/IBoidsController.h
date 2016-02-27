#pragma once

#include "GameController.h"
#include "MarchingCubes.h"

#include <queue>

class IBoidsController : public GameController
{
public:
	IBoidsController();
	~IBoidsController();

	// GameController Interface
	virtual void Initialize() override;
	virtual void Update(const GLfloat& dt) override;
	virtual void ProcessInput(const GLint& Key, const GLint& Action, const GLint& Mode) override;
	virtual void ProcessMouseMove(const GLdouble& dX, const GLdouble& dY) override;
	virtual void Render() override;

private:

	// Structure to contain blob data
	struct Blob
	{
		Blob(const glm::vec3& Velocity, const glm::vec3& GridPos, const GLfloat Radius) : Velocity(Velocity), Radius(Radius), Position(GridPos)
		{
			static GLuint id = 1;

			BID = id++;
			RadiusSquared = glm::pow(Radius, 2);
		};

		GLboolean Blob::operator==(const Blob &Other) const
		{
			return this->BID == Other.BID;
		}

		GLboolean Blob::operator!=(const Blob &Other) const
		{
			return !(*this == Other);
		}

		GLfloat Radius, RadiusSquared;
		glm::vec3 Position, Velocity;
		GLuint BID;
	};

	// Functions for computing the mesh
	void ComputeVoxels();
	GLfloat ComputeAtGrid(const GLuint& ix, const GLuint& iy, const GLuint& iz);
	void AddNeighbours(const GLuint& gx, const GLuint& gy, const GLuint& gz);
	void ComputeNeighbours(const GLuint& gx, const GLuint& gy, const GLuint& gz);

	void InitBoids();

	// State
	const GLuint Resolution = 128;
	std::vector<std::vector<std::vector<GLfloat>>> Grid;
	std::vector<Blob> Blobs;

	// Buffers to refrain from allocating new memory every frame...
	std::vector<std::vector<std::vector<GLboolean>>> IsComputed;
	std::queue<glm::vec3> Neighbours;

	MarchingCubes* MeshBuilder; // Computes the mesh, retrieve vertices, indices and normals from here
	std::map<std::string, GLuint> Buffers; // Buffer storage

	// Boid functions
	void UpdateBoids(const GLfloat& dt);
	glm::vec3 Alignment(Blob& B);
	glm::vec3 Cohesion(Blob& B);
	glm::vec3 Seperation(Blob& B);
};

