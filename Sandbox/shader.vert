#version 450 core

layout (location = 0) in vec3 aPos;

out vec3 vPos;

uniform mat4 uMVP;

void main() {
	gl_Position = uMVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vPos = aPos;
}