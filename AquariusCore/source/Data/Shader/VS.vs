//VERTEXSHADER______________________
#TYPE VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_VP;
uniform mat4 u_transform;

out vec3 vPos;


void main()
{
	vPos=aPos;
	gl_Position = u_VP*u_transform*vec4(aPos, 1.0);
}
//______________________________________________