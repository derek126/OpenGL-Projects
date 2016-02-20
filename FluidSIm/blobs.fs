#version 330 core

in VS_OUT
{
    vec3 Normal;
	vec3 FragPos;
} fs_in;

layout (std140) uniform SceneData
{
    mat4 Projection;
    mat4 View;
	vec3 LightDirection;
	vec3 LightColor;
};

uniform samplerCube BlobsPortal;
uniform vec3 CameraPosition;

out vec4 FragColor;

void main()
{
	vec3 LightDir = vec3(View * vec4(LightDirection, 0.f));

	vec3 I = normalize(fs_in.FragPos - CameraPosition);
    vec3 R = refract(I, normalize(-fs_in.Normal), (1.f / 1.33f));

	float AmbientStrength = 0.1f;
    vec3 Ambient = AmbientStrength * LightColor;
	vec3 Diffuse =  max(dot(normalize(fs_in.Normal), LightDir), 0.0) * LightColor;

	float SpecularStrength = 0.5f;
	float Spec = pow(max(dot(normalize(-fs_in.FragPos), reflect(LightDir, normalize(fs_in.Normal))), 0.0), 32);
	vec3 Specular = SpecularStrength * Spec * LightColor; 

    FragColor = vec4(Ambient + Diffuse + Specular, 0.95f) * texture(BlobsPortal, R);
} 