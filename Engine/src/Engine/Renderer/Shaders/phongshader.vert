#version 410 core

layout(location = 0) in vec4 positionIn;    // 1st attribute buffer = vertex positions
layout(location = 1) in vec3 normalIn;
layout(location = 2) in vec2 uvIn;
layout(location = 3) in vec3 colorIn;       // 2nd attribute buffer = colors

out vec3 fragPos;
out vec4 color;
out vec3 normalTransposed;
out vec2 UV;

uniform mat4 model;                         //the matrix for the model
uniform mat4 view;                          //View matrix from Camera
uniform mat4 projection;                    //projection matrix from Camera

void main()
{
   vec4 worldPos = model * positionIn;
   gl_Position = projection * view * worldPos;       //calculate the position of the model
   normalTransposed = vec3(transpose(inverse(model)) * vec4(normalIn,0.0));
   color = vec4(colorIn,1);
   fragPos = worldPos.xyz;
   UV = uvIn;
}
