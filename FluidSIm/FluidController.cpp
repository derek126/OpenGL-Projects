#include "FluidController.h"
#include "ResourceManager.h"

FluidController::FluidController() :
	Scale(5.f / GridSize),
	Voxel(nullptr)
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

	Blobs.push_back(Blob(glm::vec3(GridToWorldX(GridSize / 2), GridToWorldY(GridSize / 2), GridToWorldZ(GridSize / 2))));
	Blobs.push_back(Blob(glm::vec3(GridToWorldX(GridSize / 2), GridToWorldY(GridSize / 2), GridToWorldZ(GridSize / 2))));
	Blobs.push_back(Blob(glm::vec3(GridToWorldX(GridSize / 2), GridToWorldY(GridSize / 2), GridToWorldZ(GridSize / 2))));
	Blobs.push_back(Blob(glm::vec3(GridToWorldX(GridSize / 2), GridToWorldY(GridSize / 2), GridToWorldZ(GridSize / 2))));
	Blobs.push_back(Blob(glm::vec3(GridToWorldX(GridSize / 2), GridToWorldY(GridSize / 2), GridToWorldZ(GridSize / 2))));
	Blobs.push_back(Blob(glm::vec3(GridToWorldX(GridSize / 2), GridToWorldY(GridSize / 2), GridToWorldZ(GridSize / 2))));
}

FluidController::~FluidController()
{
	if (Voxel) delete Voxel;

	glDeleteVertexArrays(1, &Buffers["VAO"]);
	glDeleteBuffers(1, &Buffers["Vertices"]);
	glDeleteBuffers(1, &Buffers["Normals"]);
	glDeleteBuffers(1, &Buffers["EBO"]);
}

void FluidController::Initialize()
{
	// Increase screen dimensions and then set the camera location
	SetScreenDimensions(1920, 1080);

	Camera->SetPosition(glm::vec3(50.f, 80.f, 80.0f));
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

	GLuint VAO, EBO, Normals, Vertices;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &Vertices);
	glGenBuffers(1, &Normals);

	Buffers["VAO"] = VAO;
	Buffers["Vertices"] = Vertices;
	Buffers["Normals"] = Normals;
	Buffers["EBO"] = EBO;

	// Initialize the shader to be used for the masses
    RESOURCEMANAGER.LoadShader("phong.vs", "phong.fs", nullptr, "Phong");
	RESOURCEMANAGER.GetShader("Phong").SetVector3f("Color", glm::vec3(0.f, 0.75f, 1.f), true);
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
	static GLint normalX = 1920 / 2;
	static GLint normalY = 1080 / 2;

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

	RESOURCEMANAGER.GetShader("Phong").Use();
	glDrawElements(GL_TRIANGLES, Mesh.GetIndices().size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
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
