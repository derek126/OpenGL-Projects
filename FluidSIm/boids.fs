#version 430 core

in VS_OUT
{
    vec3 Normal;
	vec3 FragPos;
} fs_in;

out vec4 FragColor;

layout (std140) uniform SceneData
{
    mat4 Projection;
    mat4 View;
	vec3 LightDirection;
	vec3 LightColor;
};

uniform vec3 Color;
uniform vec3 CameraPosition;

void main()
{
	vec3 LightDir = vec3(View * vec4(LightDirection, 0.f));

	float AmbientStrength = 0.1f;
    vec3 Ambient = AmbientStrength * LightColor;
	vec3 Diffuse =  max(dot(normalize(fs_in.Normal), LightDir), 0.0) * LightColor;

	float SpecularStrength = 1.f;
	float Spec = pow(max(dot(normalize(fs_in.FragPos), reflect(LightDir, normalize(fs_in.Normal))), 0.0), 128);
	vec3 Specular = SpecularStrength * Spec * LightColor; 

    vec3 Result = (Ambient + Diffuse + Specular) * Color;
    FragColor = vec4(Result, 1.0f);
} 