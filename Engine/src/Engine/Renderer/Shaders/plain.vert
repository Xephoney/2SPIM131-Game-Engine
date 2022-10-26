#version 410 core
   
layout(location = 0) in vec3 positionIn;

out vec3 v_pos;

void main()
{
    gl_Position = vec4(positionIn,1);
    v_pos = positionIn;
}