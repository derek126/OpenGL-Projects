#include "IBoidsController.h"
#include <ResourceManager.h>

#include <iostream>
#include <glm\gtc\random.hpp>

#define ScreenX 1920
#define ScreenY 1080

#define MAX_SPEED 16.f
#define RADIUS 3.f
#define NUM_BLOBS 32

#define NEIGHBOURHOOD RADIUS * 8.f
#define SEPERATION_DIST NEIGHBOURHOOD * 2.f
#define AVOID_EDGE_FACTOR 64.f // Best not to change this or they may go out of bounds
#define ALIGNMENT_FACTOR 4.f
#define COHESION_FACTOR 2.f
#define SEPERATION_FACTOR 32.f

IBoidsController::IBoidsController() :
	bAli(GL_FALSE), 
	bCoh(GL_FALSE), 
	bSep(GL_FALSE),
	bCubes(GL_FALSE)
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
	GAMEMANAGER.SetTargetFrametime(25);

	// Increase screen dimensions and then set the camera location
	SetScreenDimensions(ScreenX, ScreenY);

	Camera->SetPosition(glm::vec3(0.f, 128.f, 128.0f));
	Camera->SetFocus(glm::vec3(Resolution / 2.f, 10.f, 0.f));
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

	if (!bCubes)
	{
		MeshBuilder->ClearMesh(); // Clear previous mesh
		ComputeVoxels(); // Create new mesh
		MeshBuilder->CalculateNormals(Grid); // Calculate smoothed normals
	}
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
	if (Key == GLFW_KEY_DOWN && Action == GLFW_RELEASE)
	{
		bCubes = !bCubes;
		std::cout << "Cubes: " << (bCubes ? "On" : "Off") << std::endl;

		if (!bCubes)
		{
			// Reset blob transform
			glm::mat4 Model; // Identity
			RESOURCEMANAGER.GetShader("Boids").SetMatrix4("Model", Model, true);
		}
	}

	if (Key == GLFW_KEY_LEFT && Action == GLFW_RELEASE)
	{
		bAli = !bAli;
		std::cout << "Alignment: " << (bAli ? "On" : "Off") << std::endl;
	}

	if (Key == GLFW_KEY_UP && Action == GLFW_RELEASE)
	{
		bCoh = !bCoh;
		std::cout << "Cohesion: " << (bCoh ? "On" : "Off") << std::endl;
	}

	if (Key == GLFW_KEY_RIGHT && Action == GLFW_RELEASE)
	{
		bSep = !bSep;
		std::cout << "Sepration: " << (bSep ? "On" : "Off") << std::endl;
	}

	if (Key == GLFW_KEY_SPACE && Action == GLFW_RELEASE)
	{
		Camera->SetFocus(glm::vec3(Resolution / 2.f, -10.f, 0.f));
		Camera->UpdateView();
	}
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

#include <Cube.h>
void IBoidsController::Render()
{
	// Render boids
	if (!bCubes)
	{
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
	else
	{
		Cube cube;
		for (GLuint i = 0; i < Blobs.size(); i++)
		{
			cube.SetTranslation(Blobs[i].Position);
			cube.SetShader("Boids");
			cube.Draw();
		}
	}
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
	GLfloat R = static_cast<GLfloat>(Resolution);
	for (GLuint i = 0; i < NUM_BLOBS; i++)
	{
		Blobs.push_back(Blob(glm::vec3(glm::linearRand(-MAX_SPEED, MAX_SPEED), glm::linearRand(-MAX_SPEED, MAX_SPEED), glm::linearRand(-MAX_SPEED, MAX_SPEED)), glm::vec3(glm::linearRand(32.f, R - 32.f), glm::linearRand(32.f, R - 32.f), glm::linearRand(32.f, R - 32.f)), RADIUS));
	}
	//Blobs.push_back(Blob(glm::vec3(10.f, 0.f, 0.f), glm::vec3(Resolution / 2.f), RADIUS));

	// Set blob transform
	glm::mat4 Model; // Identity
	RESOURCEMANAGER.GetShader("Boids").SetMatrix4("Model", Model, true);
}

void IBoidsController::UpdateBoids(const GLfloat& dt)
{
	for (GLuint i = 0; i < Blobs.size(); i++)
	{
		Blobs[i].Acceleration += (AvoidEdge(Blobs[i]) + Alignment(Blobs[i]) + Cohesion(Blobs[i]) + Seperation(Blobs[i]));
		Blobs[i].Velocity += Blobs[i].Acceleration * dt;

		// Maintain a velocity between the preset MAX
		GLfloat Mag = glm::length(Blobs[i].Velocity);
		if (Mag > MAX_SPEED)
		{
			Blobs[i].Velocity = glm::normalize(Blobs[i].Velocity) * MAX_SPEED;
		}

		// Move the blob
		Blobs[i].Position += Blobs[i].Velocity * dt;
		Blobs[i].Acceleration = glm::vec3(0.f);
	}
}

glm::vec3 IBoidsController::Alignment(Blob& B)
{
	if (!bAli) return glm::vec3(0.f);

	glm::vec3 A(0.f);
	GLfloat Num = 0, Distance;
	for (GLuint i = 0; i < Blobs.size(); i++)
	{
		if (B != Blobs[i])
		{
			Distance = glm::distance(B.Position, Blobs[i].Position);
			if (Distance > 0 && Distance < NEIGHBOURHOOD)
			{
				A += Blobs[i].Velocity;
				Num++;
			}
		}
	}

	if (Num > 0)
	{
		A = A / Num;
		return A * ALIGNMENT_FACTOR;
	}

	return glm::vec3(0.f);
}

glm::vec3 IBoidsController::Cohesion(Blob& B)
{
	if (!bCoh) return glm::vec3(0.f);

	glm::vec3 C(0.f);
	GLfloat Num = 0, Distance;
	for (GLuint i = 0; i < Blobs.size(); i++)
	{
		if (B != Blobs[i])
		{
			Distance = glm::distance(B.Position, Blobs[i].Position);
			if (Distance > 0 && Distance < NEIGHBOURHOOD)
			{
				C += Blobs[i].Position;
				Num++;
			}
		}
	}

	if (Num > 0)
	{
		C = C / Num;
		return (C - B.Position) * COHESION_FACTOR;
	}

	return glm::vec3(0.f);
}

glm::vec3 IBoidsController::Seperation(Blob& B)
{
	if (!bSep) return glm::vec3(0.f);

	glm::vec3 S(0.f);
	GLfloat Num = 0, Distance;
	for (GLuint i = 0; i < Blobs.size(); i++)
	{
		if (B != Blobs[i])
		{
			Distance = glm::distance(B.Position, Blobs[i].Position);
			if (Distance > 0 && Distance < SEPERATION_DIST)
			{
				S -= (Blobs[i].Position - B.Position) / Distance;
				Num++;
			}
		}
	}

	if (Num > 0)
	{
		S = S / Num;
		return S * SEPERATION_FACTOR;
	}

	return glm::vec3(0.f);
}

glm::vec3 IBoidsController::AvoidEdge(Blob& B)
{
	glm::vec3 E(0.f);
	GLfloat Distance;
	glm::vec3 Vec;

	// Avoid +x
	Vec = glm::vec3(RADIUS * 2, B.Position.y, B.Position.z) - B.Position;
	Distance = glm::length(Vec);
	if (Distance < NEIGHBOURHOOD)
	{
		E -= Vec / Distance;
	}

	// Avoid -x
	Vec = glm::vec3(Resolution - RADIUS * 2 - 1, B.Position.y, B.Position.z) - B.Position;
	Distance = glm::length(Vec);
	if (Distance < NEIGHBOURHOOD)
	{
		E -= Vec / Distance;
	}

	// Avoid +y
	Vec = glm::vec3(B.Position.x, RADIUS * 2, B.Position.z) - B.Position;
	Distance = glm::length(Vec);
	if (Distance < NEIGHBOURHOOD)
	{
		E -= Vec / Distance;
	}

	// Avoid -y
	Vec = glm::vec3(B.Position.x, Resolution - RADIUS * 2 - 1, B.Position.z) - B.Position;
	Distance = glm::length(Vec);
	if (Distance < NEIGHBOURHOOD)
	{
		E -= Vec / Distance;
	}

	// Avoid +z
	Vec = glm::vec3(B.Position.x, B.Position.y, RADIUS * 2) - B.Position;
	Distance = glm::length(Vec);
	if (Distance < NEIGHBOURHOOD)
	{
		E -= Vec / Distance;
	}

	// Avoid -z
	Vec = glm::vec3(B.Position.x, B.Position.y, Resolution - RADIUS * 2 - 1) - B.Position;
	Distance = glm::length(Vec);
	if (Distance < NEIGHBOURHOOD)
	{
		E -= Vec / Distance;
	}

	return E * AVOID_EDGE_FACTOR;
}