#version 330 core

out vec4 fragColor;
uniform vec4 Color;

void main()
{
   fragColor = vec4(Color);
} 