#include "FluidController.h"
#include "ResourceManager.h"

#include <glm\gtc\random.hpp>

#define ScreenX 1920
#define ScreenY 1080

#define MIN_VELOCITY -5.f
#define MAX_VELOCITY 5.f
#define MAIN_RADIUS 5.f
#define SECONDARY_RADIUS 2.5f

FluidController::FluidController()
{
	Grid.resize(Resolution);
	for (unsigned i = 0; i < Resolution; i++)
	{
		Grid[i].resize(Resolution);
		for (unsigned j = 0; j < Resolution; j++)
		{
			Grid[i][j].resize(Resolution);
		}
	}
}

FluidController::~FluidController()
{
	glDeleteVertexArrays(1, &Buffers["VAO"]);
	glDeleteVertexArrays(1, &Buffers["SkyboxVAO"]);

	glDeleteBuffers(1, &Buffers["Vertices"]);
	glDeleteBuffers(1, &Buffers["Normals"]);
	glDeleteBuffers(1, &Buffers["EBO"]);
	glDeleteBuffers(1, &Buffers["SkyboxVBO"]);
}

void FluidController::Initialize()
{
	GameController::Initialize();

	// Increase screen dimensions and then set the camera location
	SetScreenDimensions(ScreenX, ScreenY);

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
	// Wait for the scene to fully load
	static GLdouble accum = 0;
	if (accum < 5.f)
	{
		accum += dt;
		return;
	}

	for (GLuint i = 1; i < Blobs.size(); i++)
	{
		if (glm::length(Blobs[i].Position - Blobs[0].Position) >= Blobs[0].Radius + SECONDARY_RADIUS * 2)
		{
			Blobs[i].Velocity = -Blobs[i].Velocity;
		}
		Blobs[i].Position += Blobs[i].Velocity * static_cast<float>(dt);
	}

	// Compute field strength at each
	for (GLuint i = 0; i < Resolution; i++)
	{
		for (GLuint j = 0; j < Resolution; j++)
		{
			for (GLuint k = 0; k < Resolution; k++)
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
	GLfloat val = 0;
	for (unsigned i = 0; i < Blobs.size(); i++)
	{
		val += glm::pow(Blobs[i].Radius, 2) / (glm::pow(gx - Blobs[i].Position.x, 2) + glm::pow(gy - Blobs[i].Position.y, 2) + glm::pow(gz - Blobs[i].Position.z, 2));
	}

	return val;
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

	// Add blobs
	Blobs.push_back(Blob(glm::vec3(0.f), glm::vec3(Resolution / 2.f), MAIN_RADIUS)); // Main blob

	Blobs.push_back(Blob(glm::vec3(glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY)), glm::vec3(Resolution / 2.f), SECONDARY_RADIUS));
	Blobs.push_back(Blob(glm::vec3(glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY)), glm::vec3(Resolution / 2.f), SECONDARY_RADIUS));
	Blobs.push_back(Blob(glm::vec3(glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY)), glm::vec3(Resolution / 2.f), SECONDARY_RADIUS));
	Blobs.push_back(Blob(glm::vec3(glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY)), glm::vec3(Resolution / 2.f), SECONDARY_RADIUS));
	Blobs.push_back(Blob(glm::vec3(glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY)), glm::vec3(Resolution / 2.f), SECONDARY_RADIUS));
	Blobs.push_back(Blob(glm::vec3(glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY)), glm::vec3(Resolution / 2.f), SECONDARY_RADIUS));
	Blobs.push_back(Blob(glm::vec3(glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY)), glm::vec3(Resolution / 2.f), SECONDARY_RADIUS));
	Blobs.push_back(Blob(glm::vec3(glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY)), glm::vec3(Resolution / 2.f), SECONDARY_RADIUS));
	Blobs.push_back(Blob(glm::vec3(glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY)), glm::vec3(Resolution / 2.f), SECONDARY_RADIUS));
	Blobs.push_back(Blob(glm::vec3(glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY)), glm::vec3(Resolution / 2.f), SECONDARY_RADIUS));
	Blobs.push_back(Blob(glm::vec3(glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY), glm::linearRand(MIN_VELOCITY, MAX_VELOCITY)), glm::vec3(Resolution / 2.f), SECONDARY_RADIUS));

	RESOURCEMANAGER.GetShader("Blobs").SetVector3f("MainBlobPos", Blobs[0].Position);
}
