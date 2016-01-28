#include "FluidController.h"
#include "ResourceManager.h"

#include <iostream>

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
}

FluidController::~FluidController()
{
	if (Voxel) delete Voxel;
}

void FluidController::Initialize()
{
	// Increase screen dimensions and then set the camera location
	SetScreenDimensions(1920, 1080);
	Camera->SetPosition(glm::vec3(8.f, 8.f, 10.0f));
	Camera->SetLookAt(glm::vec3(1.f, 0.f, 0.f));
	Camera->SetWorldUp(glm::vec3(0.f, 1.f, 0.f));
	Camera->UpdateView();

	//SetCamera(glm::vec3(8.f, 8.f, 10.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.0f, 0.f)); // Real
	//SetCamera(glm::vec3(75.f, 55.f, 150.0f), glm::vec3(10.f, 20.f, 0.f), glm::vec3(0.f, 1.0f, 0.f)); // 64
	//SetCamera(glm::vec3(75.f, 55.f, 50.0f), glm::vec3(5.f, 5.f, 0.f), glm::vec3(0.f, 1.0f, 0.f)); // 32

	// OpenGL configuration
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	// Initialize the shader to be used for the masses
	RESOURCEMANAGER.LoadShader("phong.vs", "phong.fs", nullptr, "Phong");
	RESOURCEMANAGER.GetShader("Phong").SetVector3f("Color", glm::vec3(0.f, 0.75f, 1.f), true);

	Voxel = new Cube();
	Voxel->SetShader("Phong");
	Voxel->SetScale(glm::vec3(Scale, Scale, Scale));
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
}

void FluidController::ProcessInput(const GLdouble& dt)
{
}

void FluidController::Render()
{
	auto Draw = [this](const GLint& i, const GLint& j, const GLint& k)
	{
		Voxel->SetTranslation(glm::vec3(i * Scale, j * Scale, k * Scale)); // Real
		//Voxel->SetTranslation(glm::vec3(i, j, k)); // Testing
		Voxel->Draw();
	};

	for (GLuint i = 0; i < GridSize; i++)
	{
		for (GLuint j = 0; j < GridSize; j++)
		{
			for (GLuint k = 0; k < GridSize; k++)
			{
				/*if (Grid[i][j][k] >= 1.f)
				{
					//Voxel->SetTranslation(glm::vec3(i * Scale, j * Scale, k * Scale));
					//Voxel->Draw(View, Projection);
					//Draw(i, j, k);
				}*/

				if (Grid[i][j][k] >= 1.f)
				{
					if (i == 0 || i == GridSize - 1 ||
						j == 0 || j == GridSize - 1 ||
						k == 0 || k == GridSize - 1)
					{
						Draw(i, j, k);
					}
					else
					{
						if (Grid[i + 1][j][k] < 1.f ||
							Grid[i - 1][j][k] < 1.f ||
							Grid[i][j + 1][k] < 1.f ||
							Grid[i][j - 1][k] < 1.f ||
							Grid[i][j][k + 1] < 1.f ||
							Grid[i][j][k - 1] < 1.f)
						{
							Draw(i, j, k);
						}
					}
				}
			}
		}
	}
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
