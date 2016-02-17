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

	glDeleteVertexArrays(1, &Buffers["GrassVAO"]);
	glDeleteBuffers(1, &Buffers["GrassVBO"]);
	glDeleteBuffers(1, &Buffers["GrassEBO"]);
	glDeleteBuffers(1, &Buffers["GrassInstanceData"]);
}

void FluidController::Initialize()
{
	GameController::Initialize();

	// Increase screen dimensions and then set the camera location
	SetScreenDimensions(ScreenX, ScreenY);

	Camera->SetPosition(glm::vec3(45.f, 10.f, 45.0f));
	Camera->SetFocus(glm::vec3(0.5f, 0.5f, 0.f));
	Camera->SetWorldUp(glm::vec3(0.f, 1.f, 0.f));
	Camera->UpdateView();

	// Hide the mouse cursor
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Set the directional light direction and color
	RESOURCEMANAGER.SetLightDirection(glm::vec3(-1.f, -1.f, -1.f));
	RESOURCEMANAGER.SetLightColor(glm::vec3(1.f, 1.f, 1.f));

	// OpenGL configuration
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	InitBlobs();
	InitSkybox();
	InitGrass();
}

void FluidController::Update(const GLdouble& dt)
{
	// Wait for the scene to fully load
	static GLdouble accum = 0;
	accum += dt;
	if (accum < 2.5f)
	{
		return;
	}
	RESOURCEMANAGER.GetShader("Grass").SetFloat("Disp", static_cast<GLfloat>(glm::sin(accum / 2.f) / 4.f), true);

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
	// Render Grass
	glDisable(GL_CULL_FACE);
	glBindVertexArray(Buffers["GrassVAO"]);
	RESOURCEMANAGER.GetShader("Grass").Use();
	//RESOURCEMANAGER.GetShader("Grass").SetFloat("Disp", 1.f, true);
	RESOURCEMANAGER.GetTexture2D("Grass").Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0, 14400);
	glBindVertexArray(0);
	glEnable(GL_CULL_FACE);

	//Render skybox
	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	glBindVertexArray(Buffers["SkyboxVAO"]);
	RESOURCEMANAGER.GetShader("Skybox").Use();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // Set depth function back to default

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
	Images["Right"] = "Assets/right.bmp";
	Images["Left"] = "Assets/left.bmp";
	Images["Top"] = "Assets/top.bmp";
	Images["Bottom"] = "Assets/bottom.bmp";
	Images["Back"] = "Assets/back.bmp";
	Images["Front"] = "Assets/front.bmp";
	RESOURCEMANAGER.LoadTexture3D(Images, false, "Skybox").SetWrapS(GL_CLAMP_TO_EDGE).SetWrapT(GL_CLAMP_TO_EDGE).SetWrapR(GL_CLAMP_TO_EDGE).Bind();

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
}

void FluidController::InitGrass()
{
	GLfloat vertices[] = {
		// Positions            // Texture Coords   // Normals
		0.5f,  0.5f, 0.0f,		1.0f, 1.0f,		0.f, -1.f, 0.f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.f, -1.f, 0.f,
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.f, -1.f, 0.f,
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,		0.f, -1.f, 0.f,

		0.5f,  0.5f, 0.5f,		1.0f, 1.0f,		0.f, -1.f, 0.f,
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f,		0.f, -1.f, 0.f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		0.f, -1.f, 0.f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,		0.f, -1.f, 0.f,

		0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		0.f, -1.f, 0.f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		0.f, -1.f, 0.f,
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		0.f, -1.f, 0.f,
		-0.5f,  0.5f, 0.5f,		0.0f, 1.0f,		0.f, -1.f, 0.f
	};

	GLuint indices[] = {
		3, 1, 0,
		3, 2, 1,

		7, 5, 4,
		7, 6, 5,

		11, 9, 8,
		11, 10, 9
	};

	// Create and store buffers
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	Buffers["GrassVAO"] = VAO;
	Buffers["GrassVBO"] = VBO;
	Buffers["GrassEBO"] = EBO;

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);

	// Tex coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	// Normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));

	// Setup the offsets for the instances of the grass
	std::array<glm::mat4, 14400> ModelMats;
	for (GLuint i = 0; i < 120; i++)
	{
		for (GLuint j = 0; j < 120; j++)
		{
			ModelMats[120 * i + j] = glm::translate(glm::mat4(), glm::vec3(static_cast<GLint>(i) * 5 - 464, glm::sin(i / 8.f + 10) * glm::sin(j / 8.f + 10) * 10, static_cast<GLint>(j) * 5 - 464));
			ModelMats[120 * i + j] = glm::rotate(ModelMats[120 * i + j], glm::linearRand(-10.f, 10.f), glm::vec3(0.f, 1.f, 0.f));
			ModelMats[120 * i + j] = glm::scale(ModelMats[120 * i + j], glm::vec3(8.f, 8.f, 8.f));
		}
	}

	GLuint GrassInstanceData;
	glGenBuffers(1, &GrassInstanceData);
	glBindBuffer(GL_ARRAY_BUFFER, GrassInstanceData);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * ModelMats.size(), ModelMats.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

	Buffers["GrassInstanceData"] = GrassInstanceData;

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Initialize the shader to be used for the grass
	RESOURCEMANAGER.LoadShader("grass.vs", "grass.fs", nullptr, "Grass");
	RESOURCEMANAGER.LoadTexture2D("Assets/billboardgrass.png", GL_TRUE, "Grass").SetWrapS(GL_CLAMP_TO_EDGE).SetWrapT(GL_CLAMP_TO_EDGE);
}
