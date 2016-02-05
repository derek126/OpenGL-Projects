#version 330 core

in VS_OUT
{
    vec3 Normal;
	vec3 FragPos;
} fs_in;

uniform samplerCube Skybox;
uniform vec3 CameraPosition;

out vec4 FragColor;

void main()
{
	vec3 I = normalize(fs_in.FragPos - CameraPosition);
    vec3 R = refract(I, normalize(fs_in.Normal), (1.f / 1.33f));
    FragColor = texture(Skybox, R);
} 