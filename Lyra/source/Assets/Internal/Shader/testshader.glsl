//VERTEXSHADER______________________
#TYPE VERTEX
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
//______________________________________________


//FRAGMENTSHADER______________________
#TYPE FRAGMENT
#version 460 core
layout(location = 0) out vec4 OutColor1;
layout(location = 1) out vec4 OutColor2;



in vec3 ourColor;
in vec2 TexCoord;



void main()
{
	OutColor1  = vec4(1.0,1.0,1.0,1.0);
    OutColor2  = vec4(0.8,0.3,0.5,1.0);
}
//___________________________________