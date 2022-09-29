#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_color;

uniform mat4 u_VP;

out vec3 v_Position;
out vec4 v_color;

void main()
{
    v_Position = a_Position;
    v_color = a_color;
    gl_Position = u_VP * vec4(a_Position, 1.0);

}