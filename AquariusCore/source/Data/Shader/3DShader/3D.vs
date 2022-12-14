//VERTEXSHADER_________________________
#TYPE VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 u_VP;
uniform mat4 u_transform;

void main()
{
	gl_Position = u_VP *u_transform*vec4(aPos, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
//____________________________________