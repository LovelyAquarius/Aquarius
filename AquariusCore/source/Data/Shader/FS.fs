//FRAGMENTSHADER______________________
#TYPE FRAGMENT
#version 330 core
layout (location = 0) out vec4 FragColor;

in vec3 vPos;

uniform vec3 u_color;

void main()
{
    FragColor =vec4(u_color,1.0f);
}
//___________________________________