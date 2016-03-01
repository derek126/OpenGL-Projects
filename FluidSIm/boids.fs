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

uniform vec3 CameraPosition;

vec3 AmbientColor = vec3(1.f, 0.15f, 0.f);
vec3 DiffuseColor = vec3(1.f, 0.27f, 0.f);
vec3 SpecularColor = vec3(1.f, 0.39f, 0.28f);
float Shininess = 32;

// Red rubber
//vec3 AmbientColor = vec3(0.5f, 0.f, 0.f);
//vec3 DiffuseColor = vec3(0.5f, 0.4f, 0.4f);
//vec3 SpecularColor = vec3(0.7f, 0.04f, 0.04f);
//float Shininess = 32;

// Red Plastic
//vec3 AmbientColor = vec3(0.1745f, 0.01175f, 0.01175f);
//vec3 DiffuseColor = vec3(0.61424f, 0.04136f, 0.04136f);
//vec3 SpecularColor = vec3(0.7f, 0.6f, 0.6f);
//float Shininess = 128;

// Ruby
//vec3 AmbientColor = vec3(0.f, 0.f, 0.f);
//vec3 DiffuseColor = vec3(0.5f, 0.f, 0.f);
//vec3 SpecularColor = vec3(0.727811f, 0.626959f, 0.626959f);
//float Shininess = 256;

void main()
{
	vec3 LightDir = vec3(View * vec4(LightDirection, 0.f));

    vec3 Ambient = LightColor * AmbientColor;

	vec3 Diffuse =  max(dot(normalize(fs_in.Normal), LightDir), 0.0) * LightColor * DiffuseColor;

	float Spec = pow(max(dot(normalize(fs_in.FragPos), reflect(LightDir, normalize(fs_in.Normal))), 0.0), Shininess);
	vec3 Specular =  Spec * LightColor * SpecularColor; 

    FragColor = vec4((Ambient + Diffuse + Specular), 1.0f);
} 