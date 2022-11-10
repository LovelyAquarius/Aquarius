//VERTEXSHADER______________________
#TYPE VERTEX
#version 330 core
layout (location = 0)  in vec4  aPosition;
layout (location = 1)   in vec4  aColor;
layout (location = 2)  in vec2  aTexCoord;
layout (location = 3)  in float  aTexIndex;
layout (location = 4)  in float  aTilingFactor;

uniform mat4 u_VP;
uniform mat4 u_transform;

out vec4 vColor;
out vec2 vTexCoord;
out float vTexIndex;
out float vTilingFactor;

void main()
{
	vColor = aColor;
	vTexCoord = aTexCoord;
	vTexIndex = aTexIndex;
	vTilingFactor = aTilingFactor;
	gl_Position = u_VP*aPosition;
}
//______________________________________________


//FRAGMENTSHADER______________________
#TYPE FRAGMENT
#version 330 core
layout (location = 0) out vec4 FragColor;

in vec4 vColor;
in vec2 vTexCoord;
in float vTexIndex;
in float vTilingFactor;

uniform vec4 u_color;
uniform sampler2D u_Texture[32];

void main()
{
    FragColor =texture(u_Texture[int(vTexIndex)], vTexCoord*vTilingFactor)*vColor;
}
//___________________________________