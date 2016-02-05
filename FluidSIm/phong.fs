#version 330 core

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
};

uniform vec3 Color;

void main()
{
	float AmbientStrength = 0.1f;
	vec3 LightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 LightPos = vec3(View * vec4(vec3(0.f, 0.f, 50.f), 1.0f));

    vec3 Ambient = AmbientStrength * LightColor;

	vec3 LightDirection = normalize(LightPos - fs_in.FragPos);
	vec3 Diffuse =  max(dot(normalize(fs_in.Normal), LightDirection), 0.0) * LightColor;

	float SpecularStrength = 0.5f;
	float Spec = pow(max(dot(normalize(-fs_in.FragPos), reflect(-LightDirection, normalize(fs_in.Normal))), 0.0), 32);
	vec3 Specular = SpecularStrength * Spec * LightColor; 

    vec3 Result = (Ambient + Diffuse + Specular) * Color;
    FragColor = vec4(Result, 1.0f);
} 