#version 450	// GLSL 4.5

layout(location = 0) in vec3 fragCol;
layout(location = 0) out vec4 outColor;

void main()
{
	outColor = vec4(fragCol, 1.0);
}
