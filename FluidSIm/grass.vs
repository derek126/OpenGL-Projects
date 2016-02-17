#version 330 core
  
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 Normal;
layout (location = 3) in mat4 Model;

layout (std140) uniform SceneData
{
    mat4 Projection;
    mat4 View;
	vec3 LightDirection;
	vec3 LightColor;
};

out VS_OUT
{
    vec3 Normal;
	vec3 FragPos;
	vec2 UV;
} vs_out;

uniform float Disp;

void main()
{
	vec3 P = Position;
	if (UV.y >= 0.9) P += vec3(Disp / 2.f, 0.f, Disp / 2.f);

    gl_Position = Projection * View * Model * vec4(P, 1.0);
	vs_out.UV = UV;
	vs_out.Normal = mat3(transpose(inverse(View * Model))) * Normal;
	vs_out.FragPos = vec3(View * Model * vec4(Position, 1.0f));
}