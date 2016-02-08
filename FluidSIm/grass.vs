#version 330 core
  
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 Normal;
layout (location = 3) in mat4 Model;

layout (std140) uniform SceneData
{
    mat4 Projection;
    mat4 View;
};

out VS_OUT
{
	vec2 UV;
} vs_out;

uniform float Disp;

void main()
{
	vec3 P = Position;
	if (UV.y >= 0.9) P += vec3(Disp, 0.f, 0.f);

    gl_Position = Projection * View * Model * vec4(P, 1.0);
	vs_out.UV = UV;
}