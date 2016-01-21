#version 330 core
  
layout (location = 0) in vec3 Position;

layout (std140) uniform SceneData
{
    mat4 Projection;
    mat4 View;
};

uniform mat4 Model;

void main()
{
    gl_Position = Projection * View * Model * vec4(Position.x, Position.y, Position.z, 1.0);
}