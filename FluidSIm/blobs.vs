#version 330 core
  
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;

layout (std140) uniform SceneData
{
    mat4 Projection;
    mat4 View;
};

out VS_OUT
{
    vec3 Normal;
	vec3 FragPos;
} vs_out;

uniform mat4 Model;

void main()
{
	vs_out.Normal = mat3(transpose(inverse(View))) * Normal;
	vs_out.FragPos = vec3(View * vec4(Position, 1.0f));

    gl_Position = Projection * View * vec4(Position, 1.0);
}