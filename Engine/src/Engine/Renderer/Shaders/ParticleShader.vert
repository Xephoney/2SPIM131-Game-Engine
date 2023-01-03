#version 450 core

layout (location = 0) in vec3 a_Position;

uniform mat4 u_view_projection;
uniform mat4 u_model;

void main()
{
	gl_Position = u_view_projection * u_model * vec4(a_Position, 1.0);
}