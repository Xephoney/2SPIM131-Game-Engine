#version 410 core

layout(location = 0) in vec3 positionIn;    // 1st attribute buffer = vertex positions


out vec3 textureCoords;                             //color sent to rest of pipeline                       //the matrix for the model

uniform mat4 k_view;
uniform mat4 k_projection;

void main()
{
    vec4 pos = k_projection * k_view * vec4(positionIn, 1.0f);

    gl_Position = pos.xyww;       //Dette gjør at skyboxen alltid er back alt i scenen
    textureCoords = vec3(positionIn.x, positionIn.y, -positionIn.z);
}
