#version 330 core

in VS_OUT
{
	vec2 UV;
} fs_in;


layout (std140) uniform SceneData
{
    mat4 Projection;
    mat4 View;
};

uniform sampler2D Grass;

out vec4 FragColor;

void main()
{
    vec4 t = texture(Grass, fs_in.UV);
	if (t.a < 0.5) discard;
	FragColor = t;
} 