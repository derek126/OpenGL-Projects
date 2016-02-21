#version 430 core

layout (location = 0) in vec3 Position;

layout (std140) uniform SceneData
{
    mat4 Projection;
    mat4 View;
};

out vec3 TexCoords;

void main()
{
	// Remove the translation component of the view matrix
    vec4 Pos = Projection * mat4(mat3(View)) * vec4(Position, 1.0);  
	gl_Position = Pos.xyww; // Depth testing trick
    TexCoords = Position;
}  