//VERTEXSHADER______________________
#TYPE VERTEX
#version 330 core
layout (location = 0) in  vec3  astart;
layout (location = 1)  in  vec3  aend;
layout (location = 2) in  vec3  acontrol;
layout (location = 3) in  float   avertexcount;
layout (location = 4)in   vec4 aColor;

uniform mat4 u_VP;
uniform mat4 u_transform;

out VS_OUT
{
    vec4 gstart;
    vec4 gend;
    vec4 gcontrol;
    float    gvertexcount;
    vec4 gColor;
} vs_out;

void main()
{
    vs_out.gstart           = u_VP*vec4(astart, 1.0);
    vs_out.gend             = u_VP*vec4(aend, 1.0);
    vs_out.gcontrol        = u_VP*vec4(acontrol, 1.0);
    vs_out.gvertexcount = avertexcount;
    vs_out.gColor           = aColor;
    gl_Position              = u_VP*vec4(acontrol, 1.0);
}
//___________________________________

//GEOMETRYSHADER______________________
#TYPE GEOMETRY
#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 100) out;


in VS_OUT {
    vec4 gstart;
    vec4 gend;
    vec4 gcontrol;
    float    gvertexcount;
    vec4 gColor;
} gs_in[];

out vec4 fColor;

vec4 CalculateQuadraticBezierPosition(const vec4 start, const vec4 end, const vec4 control, const float t)
{
    vec4 point1  = mix(start, control, t);
    vec4 point2 = mix(control, end, t);
    return  mix(point1, point2, t);
}


void main() 
{    
    fColor = gs_in[0].gColor;

    float step = 1.0 /gs_in[0].gvertexcount;
    float t = 0.0;
    for (int index = 0; index < gs_in[0].gvertexcount; index++)
    {
        gl_Position =CalculateQuadraticBezierPosition(gs_in[0].gstart, gs_in[0].gend, gs_in[0].gcontrol, t);
        EmitVertex();
        t += step;
    }
    gl_Position =gs_in[0].gend;
    EmitVertex();

    EndPrimitive();
}
//___________________________________

//FRAGMENTSHADER______________________
#TYPE FRAGMENT
#version 330 core
layout (location = 0) out vec4 FragColor;

 in vec4  fColor;

uniform vec4 u_color;

void main()
{
    FragColor =fColor;
}
//___________________________________
