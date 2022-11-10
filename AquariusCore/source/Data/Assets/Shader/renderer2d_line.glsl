//VERTEXSHADER______________________
#TYPE VERTEX
#version 330 core
layout (location = 0)  in  vec3  aPosition;
layout (location = 1)   in  vec4  aColor;

uniform mat4 u_VP;
uniform mat4 u_transform;

out vec4 vColor;
void main()
{
    vColor = aColor;
    gl_Position = u_VP*vec4(aPosition, 1.0);
}
//______________________________________________


//FRAGMENTSHADER______________________
#TYPE FRAGMENT
#version 330 core
layout (location = 0) out vec4 FragColor;

 in vec4  vColor;

uniform vec4 u_color;

void main()
{
    FragColor =vColor;
}
//___________________________________