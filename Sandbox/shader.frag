#version 450 core

in vec3 vPos;

out vec4 uColor;

void main()
{
	uColor = vec4(vPos, 1.0f);
} 