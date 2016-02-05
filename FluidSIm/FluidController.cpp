#include "FluidController.h"
#include "ResourceManager.h"

#define ScreenX 1920
#define ScreenY 1080

FluidController::FluidController() :
	Scale(5.f / GridSize)
{
	Grid.resize(GridSize);
	for (unsigned i = 0; i < GridSize; i++)
	{
		Grid[i].resize(GridSize);
		for (unsigned j = 0; j < GridSize; j++)
		{
			Grid[i][j].resize(GridSize);
		}
	}
}

FluidController::~FluidController()
{
	glDeleteVertexArrays(1, &Buffers["VAO"]);
	glDeleteBuffers(1, &Buffers["Vertices"]);
	glDeleteBuffers(1, &Buffers["Normals"]);
	glDeleteBuffers(1, &Buffers["EBO"]);
}

void FluidController::Initialize()
{
	// Increase screen dimensions and then set the camera location
	SetScreenDimensions(ScreenX, ScreenY);

	//Camera->SetPosition(glm::vec3(0.f, 0.f, 5.0f));
	Camera->SetPosition(glm::vec3(50.f, 25.f, 80.0f));
	Camera->SetFocus(glm::vec3(0.5f, 0.5f, 0.f));
	Camera->SetWorldUp(glm::vec3(0.f, 1.f, 0.f));
	Camera->UpdateView();

	// Hide the mouse cursor
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// OpenGL configuration
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	InitBlobs();
	InitSkybox();
}

void FluidController::Update(const GLdouble& dt)
{
	// Translate the metaballs based on their velocities
	for (GLuint i = 0; i < Blobs.size(); i++)
	{
		GLuint gx = WorldToGridX(Blobs[i].Position.x),
			gy = WorldToGridY(Blobs[i].Position.y),
			gz = WorldToGridZ(Blobs[i].Position.z);

		if (gx == 0 || gx >= GridSize - 1)
		{
			Blobs[i].Velocity.x = -Blobs[i].Velocity.x;
		}

		if (gy == 0 || gy >= GridSize - 1)
		{
			Blobs[i].Velocity.y = -Blobs[i].Velocity.y;
		}

		if (gz == 0 || gz >= GridSize - 1)
		{
			Blobs[i].Velocity.z = -Blobs[i].Velocity.z;
		}

		Blobs[i].Position += Blobs[i].Velocity * static_cast<float>(dt);
	}

	// Compute field strength at each
	for (GLuint i = 0; i < GridSize; i++)
	{
		for (GLuint j = 0; j < GridSize; j++)
		{
			for (GLuint k = 0; k < GridSize; k++)
			{
				Grid[i][j][k] = ComputeVoxel(i, j, k);
			}
		}
	}

	// Generate the mesh
	Mesh.CreateMesh(Grid);
}

void FluidController::ProcessInput(const GLint& Key, const GLint& Action, const GLint& Mode)
{
}

void FluidController::ProcessMouseMove(const GLdouble& dX, const GLdouble& dY)
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

#include <iostream>
#include <glm/gtx/string_cast.hpp>
void FluidController::Render()
{
	// Render blobs
	glBindVertexArray(Buffers["VAO"]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers["EBO"]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Mesh.GetIndices().size() * sizeof(GLuint), Mesh.GetIndices().data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers["Vertices"]);
	glBufferData(GL_ARRAY_BUFFER, Mesh.GetVertices().size() * 3 * sizeof(GLfloat), Mesh.GetVertices().data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers["Normals"]);
	glBufferData(GL_ARRAY_BUFFER, Mesh.GetNormals().size() * 3 * sizeof(GLfloat), Mesh.GetNormals().data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	RESOURCEMANAGER.GetShader("Blobs").SetVector3f("CameraPosition", Camera->GetPosition(), true);
	glDrawElements(GL_TRIANGLES, Mesh.GetIndices().size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Render skybox
	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	glBindVertexArray(Buffers["SkyboxVAO"]);
	RESOURCEMANAGER.GetShader("Skybox").Use();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // Set depth function back to default
}

GLfloat FluidController::ComputeVoxel(const GLuint& gx, const GLuint& gy, const GLuint& gz) const
{
	GLfloat val = 0, WorldX = GridToWorldX(gx), WorldY = GridToWorldY(gy), WorldZ = GridToWorldZ(gz);
	for (unsigned i = 0; i < Blobs.size(); i++)
	{
		val += glm::pow(Blobs[i].Radius, 2) / (glm::pow(WorldX - Blobs[i].Position.x, 2) + glm::pow(WorldY - Blobs[i].Position.y, 2) + glm::pow(WorldZ - Blobs[i].Position.z, 2));
	}

	return val;
}

GLfloat FluidController::GridToWorldX(const GLuint& gx) const
{
	return gx * Scale;
}

GLfloat FluidController::GridToWorldY(const GLuint& gy) const
{
	return gy * Scale;
}

GLfloat FluidController::GridToWorldZ(const GLuint& gz) const
{
	return gz * Scale;
}

GLuint FluidController::WorldToGridX(const GLfloat& wx) const
{
	return static_cast<GLuint>(wx / Scale);
}

GLuint FluidController::WorldToGridY(const GLfloat& wy) const
{
	return static_cast<GLuint>(wy / Scale);
}

GLuint FluidController::WorldToGridZ(const GLfloat& wz) const
{
	return static_cast<GLuint>(wz / Scale);
}

void FluidController::InitSkybox()
{
	GLfloat SkyboxVertices[] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	// Load the cubemap
	std::map<std::string, GLchar*> Images;
	Images["Right"] = "Assets/right.jpg";
	Images["Left"] = "Assets/left.jpg";
	Images["Top"] = "Assets/top.jpg";
	Images["Bottom"] = "Assets/bottom.jpg";
	Images["Back"] = "Assets/back.jpg";
	Images["Front"] = "Assets/front.jpg";
	RESOURCEMANAGER.LoadTexture3D(Images, false, "Skybox").Bind();

	// Init buffers
	GLuint SkyboxVAO, SkyboxVBO;
	glGenVertexArrays(1, &SkyboxVAO);
	glGenBuffers(1, &SkyboxVBO);

	Buffers["SkyboxVAO"] = SkyboxVAO;
	Buffers["SkyboxVBO"] = SkyboxVBO;

	glBindVertexArray(Buffers["SkyboxVAO"]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers["SkyboxVBO"]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SkyboxVertices), SkyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// Initialize the shader to be used for the skybox
	RESOURCEMANAGER.LoadShader("skybox.vs", "skybox.fs", nullptr, "Skybox");
}

void FluidController::InitBlobs()
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
	RESOURCEMANAGER.LoadShader("blobs.vs", "blobs.fs", nullptr, "Blobs");
	RESOURCEMANAGER.GetShader("Blobs").SetVector3f("Color", glm::vec3(0.f, 0.75f, 1.f), true);

	// Add blobs
	Blobs.push_back(Blob(glm::vec3(GridToWorldX(GridSize / 2), GridToWorldY(GridSize / 2), GridToWorldZ(GridSize / 2))));
	Blobs.push_back(Blob(glm::vec3(GridToWorldX(GridSize / 2), GridToWorldY(GridSize / 2), GridToWorldZ(GridSize / 2))));
	Blobs.push_back(Blob(glm::vec3(GridToWorldX(GridSize / 2), GridToWorldY(GridSize / 2), GridToWorldZ(GridSize / 2))));
	Blobs.push_back(Blob(glm::vec3(GridToWorldX(GridSize / 2), GridToWorldY(GridSize / 2), GridToWorldZ(GridSize / 2))));
	Blobs.push_back(Blob(glm::vec3(GridToWorldX(GridSize / 2), GridToWorldY(GridSize / 2), GridToWorldZ(GridSize / 2))));
	Blobs.push_back(Blob(glm::vec3(GridToWorldX(GridSize / 2), GridToWorldY(GridSize / 2), GridToWorldZ(GridSize / 2))));
}
