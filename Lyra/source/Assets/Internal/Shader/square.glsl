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

//FRAGMENTSHADER______________________
#TYPE FRAGMENT
#version 330 core
layout (location = 0) out vec4 FragColor;

in vec3 vPos;

uniform vec4 u_color;

void main()
{
    FragColor =u_color;
}
//___________________________________