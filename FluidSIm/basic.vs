#version 330 core
layout (location = 0) in vec3 Position;
  
layout (std140) uniform SceneData
{
    mat4 Projection;
    mat4 View;
};

uniform mat4 Model;

out vec4 vertexColor;

void main()
{
    gl_Position = Projection * View * vec4(Position.x, Position.y, Position.z, 1.0);
    vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);
}