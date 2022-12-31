#type vertex
#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec2 a_UV;


uniform mat4 u_view_projection;
uniform mat4 u_model;

out vec3 v_pos;
out vec4 v_color;

void main()
{
	gl_Position = u_view_projection * u_model * vec4(a_Position, 1);
	v_pos = a_Position;
	v_color = a_Color;
}

#type fragment
#version 450 core
layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out int index;

in vec3 v_pos;
in vec4 v_color;

uniform int u_ID;


void main()
{
	fragmentColor = v_color;
	index = u_ID;
}