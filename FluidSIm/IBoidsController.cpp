#include "IBoidsController.h"
#include <ResourceManager.h>

#include <thread>
#include <glm\gtc\random.hpp>

#define ScreenX 1920
#define ScreenY 1080

#define MIN_VELOCITY -10.f
#define MAX_VELOCITY 10.f
#define RADIUS 4.f
#define NUM_BLOBS 16
#define NEIGHBOURHOOD 5.f + RADIUS

IBoidsController::IBoidsController()
{
	// Create the mesh builder
	MeshBuilder = new MarchingCubes(Resolution);

	// Resize the voxel array
	Grid.resize(Resolution);
	for (GLuint i = 0; i < Resolution; i++)
	{
		Grid[i].resize(Resolution);
		for (GLuint j = 0; j < Resolution; j++)
		{
			Grid[i][j].resize(Resolution);
		}
	}

	// Resize the scratch buffer
	IsComputed.resize(Resolution);
	for (GLuint i = 0; i < Resolution; i++)
	{
		IsComputed[i].resize(Resolution);
		for (GLuint j = 0; j < Resolution; j++)
		{
			IsComputed[i][j].resize(Resolution);
		}
	}
}

IBoidsController::~IBoidsController()
{
	// Free all resources
	if (MeshBuilder) delete MeshBuilder;

	glDeleteBuffers(1, &Buffers["Vertices"]);
	glDeleteBuffers(1, &Buffers["Normals"]);
	glDeleteBuffers(1, &Buffers["EBO"]);
	glDeleteVertexArrays(1, &Buffers["VAO"]);
}

void IBoidsController::Initialize()
{
	GameController::Initialize();
	GAMEMANAGER.SetTargetFrametime(30);

	// Increase screen dimensions and then set the camera location
	SetScreenDimensions(ScreenX, ScreenY);

	Camera->SetPosition(glm::vec3(0.f, 90.f, 90.0f));
	Camera->SetFocus(glm::vec3(0.5f, 0.5f, 0.f));
	Camera->SetWorldUp(glm::vec3(0.f, 1.f, 0.f));
	Camera->UpdateView();

	// Hide the mouse cursor
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Set the directional light direction and color
	RESOURCEMANAGER.SetLightDirection(glm::vec3(0.f, 90.f, 90.0f));
	RESOURCEMANAGER.SetLightColor(glm::vec3(1.f, 1.f, 1.f));

	// OpenGL configuration
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	InitBoids();
}

void IBoidsController::Update(const GLfloat& dt)
{
	UpdateBoids(dt);

	MeshBuilder->ClearMesh(); // Clear previous mesh
	ComputeVoxels(); // Create new mesh
	MeshBuilder->CalculateNormals(Grid); // Calculate smoothed normals
}

void IBoidsController::AddNeighbours(const GLuint& gx, const GLuint& gy, const GLuint& gz)
{
	// Adds the neighbours of the given grid point
	for (GLuint x = gx - 1; x <= gx + 1; x++)
	{
		for (GLuint y = gy - 1; y <= gy + 1; y++)
		{
			for (GLuint z = gz - 1; z <= gz + 1; z++)
			{
				if (x < Resolution && x >= 0 && y < Resolution && y >= 0 && z < Resolution && z >= 0 && !IsComputed[x][y][z])
				{
					Neighbours.push(glm::vec3(x, y, z));
				}
			}
		}
	}
};

void IBoidsController::ComputeNeighbours(const GLuint& gx, const GLuint& gy, const GLuint& gz)
{
	// Computes the neighbours for each cube vertex
	for (GLuint x = gx; x <= gx + 1; x++)
	{
		for (GLuint y = gy; y <= gy + 1; y++)
		{
			for (GLuint z = gz; z <= gz + 1; z++)
			{
				if (x < Resolution && x >= 0 && y < Resolution && y >= 0 && z < Resolution && z >= 0)
				{
					if (Grid[x][y][z] == -1.f) Grid[x][y][z] = ComputeAtGrid(x, y, z);
				}
			}
		}
	}
};

void IBoidsController::ComputeVoxels()
{
	// Reset the buffers
	for (GLuint i = 0; i < Resolution; i++)
	{
		for (GLuint j = 0; j < Resolution; j++)
		{
			std::fill(Grid[i][j].begin(), Grid[i][j].end(), -1.f); // -1 denotes not calculated
			std::fill(IsComputed[i][j].begin(), IsComputed[i][j].end(), GL_FALSE);
		}
	}

	GLuint gx, gy, gz;
	for (GLuint b = 0; b < Blobs.size(); b++)
	{
		// Center point of blob
		gx = static_cast<GLuint>(Blobs[b].Position.x);
		gy = static_cast<GLuint>(Blobs[b].Position.y);
		gz = static_cast<GLuint>(Blobs[b].Position.z);

		ComputeNeighbours(gx, gy, gz);
		GLint Case = MeshBuilder->MarchCube(Grid, gx, gy, gz);
		IsComputed[gx][gy][gz] = GL_TRUE;

		// If this blobs center point is totally inside the mesh, inch upward until we find a face
		while (Case == 0)
		{
			if (++gy < Resolution && !IsComputed[gx][gy][gz])
			{
				ComputeNeighbours(gx, gy, gz);
				Case = MeshBuilder->MarchCube(Grid, gx, gy, gz);
				IsComputed[gx][gy][gz] = GL_TRUE;
				// If we found an edge, add the neighbouring points
				if (Case != 0) AddNeighbours(gx, gy, gz);
				continue;
			}
			break;
		}

		// Repeat for each neighbouring point until the mesh is built
		while (!Neighbours.empty())
		{
			glm::vec3 N = Neighbours.front();
			Neighbours.pop();

			gx = static_cast<GLuint>(N.x),
			gy = static_cast<GLuint>(N.y),
			gz = static_cast<GLuint>(N.z);

			Case = 0;
			if (!IsComputed[gx][gy][gz])
			{
				ComputeNeighbours(gx, gy, gz);
				Case = MeshBuilder->MarchCube(Grid, gx, gy, gz);
				IsComputed[gx][gy][gz] = GL_TRUE;
			}

			if (Case != 0) AddNeighbours(gx, gy, gz);
		}
	}
}

GLfloat IBoidsController::ComputeAtGrid(const GLuint& ix, const GLuint& iy, const GLuint& iz)
{
	GLfloat Influence = 0;
	// Compute the field strength at this point for all blobs
	for (GLuint b = 0; b < Blobs.size(); b++)
	{
		Influence += Blobs[b].RadiusSquared / (glm::pow(ix - Blobs[b].Position.x, 2) + glm::pow(iy - Blobs[b].Position.y, 2) + glm::pow(iz - Blobs[b].Position.z, 2));
	}

	return Influence;
}

void IBoidsController::ProcessInput(const GLint& Key, const GLint& Action, const GLint& Mode)
{
}

void IBoidsController::ProcessMouseMove(const GLdouble& dX, const GLdouble& dY)
{
	static GLint normalX = ScreenX / 2;
	static GLint normalY = ScreenY / 2;

	GLboolean bDidUpdate = false;
	if (dX != normalX || dY != normalY)
	{
		Camera->RotateByMouse(static_cast<GLfloat>(dX - normalX) * 0.0005f, static_cast<GLfloat>(dY - normalY) * 0.0005f);
		Camera->UpdateView();

		bDidUpdate = true;
	}

	if (bDidUpdate)
	{
		glfwSetCursorPos(Window, normalX, normalY);
	}
}

void IBoidsController::Render()
{
	// Render blobs
	glBindVertexArray(Buffers["VAO"]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers["EBO"]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MeshBuilder->GetIndices().size() * sizeof(GLuint), MeshBuilder->GetIndices().data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers["Vertices"]);
	glBufferData(GL_ARRAY_BUFFER, MeshBuilder->GetVertices().size() * 3 * sizeof(GLfloat), MeshBuilder->GetVertices().data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers["Normals"]);
	glBufferData(GL_ARRAY_BUFFER, MeshBuilder->GetNormals().size() * 3 * sizeof(GLfloat), MeshBuilder->GetNormals().data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	RESOURCEMANAGER.GetShader("Boids").SetVector3f("CameraPosition", Camera->GetPosition(), true);
	glDrawElements(GL_TRIANGLES, MeshBuilder->GetIndices().size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void IBoidsController::InitBoids()
{
	// Init buffers
	GLuint VAO, EBO, Normals, Vertices;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &Vertices);
	glGenBuffers(1, &Normals);

	Buffers["VAO"] = VAO;
	Buffers["Vertices"] = Vertices;
	Buffers["Normals"] = Normals;
	Buffers["EBO"] = EBO;

	// Initialize the shader to be used for the blobs
	RESOURCEMANAGER.LoadShader("boids.vs", "boids.fs", nullptr, "Boids");
	RESOURCEMANAGER.GetShader("Boids").SetVector3f("Color", glm::vec3(0.f, 0.75f, 1.f), true);

	// Add blobs
	for (GLuint i = 0; i < NUM_BLOBS; i++)
	{
		Blobs.push_back(Blob(glm::vec3(glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY)), glm::vec3(Resolution / 2.f), RADIUS));
	}
	//Blobs.push_back(Blob(glm::vec3(10.f, 0.f, 0.f), glm::vec3(Resolution / 2.f), RADIUS));

	// Set blob transform
	glm::mat4 Model;
	//Model = glm::translate(Model, glm::vec3(0.f, -20.f, 0.f));
	Model = glm::scale(Model, glm::vec3(0.5f, 0.5f, 0.5f));
	RESOURCEMANAGER.GetShader("Boids").SetMatrix4("Model", Model, true);
}

void IBoidsController::UpdateBoids(const GLfloat& dt)
{
	for (GLuint i = 0; i < Blobs.size(); i++)
	{
		// Avoid the edges of the grid
		glm::vec3 P = Blobs[i].Position;
		if (P.x <= RADIUS * 2 || P.x >= Resolution - RADIUS * 2 - 1 || P.y <= RADIUS * 2 || P.y >= Resolution - RADIUS * 2 - 1 || P.z <= RADIUS * 2 || P.z >= Resolution - RADIUS * 2 - 1)
		{
			Blobs[i].Velocity = -Blobs[i].Velocity;
		}

		// Maintain a velocity between the preset MAX and MIN
		GLfloat Mag = glm::length(Blobs[i].Velocity);
		if (Mag > MAX_VELOCITY)
		{
			Blobs[i].Velocity = glm::normalize(Blobs[i].Velocity) * MAX_VELOCITY;
		}

		if (Mag < MIN_VELOCITY)
		{
			Blobs[i].Velocity = glm::normalize(Blobs[i].Velocity) * MIN_VELOCITY;
		}

		// Move the blob
		Blobs[i].Position += Blobs[i].Velocity * dt;
	}
}

glm::vec3 IBoidsController::Alignment(Blob& B)
{
	glm::vec3 A(0.f);

	return A;
}

glm::vec3 IBoidsController::Cohesion(Blob& B)
{
	glm::vec3 C;

	return C;
}

glm::vec3 IBoidsController::Seperation(Blob& B)
{
	glm::vec3 S(0.f);

	return S;
}