#version 410 core

in vec3 textureCoords;

out vec4 fragmentColor;     //color sent to fragment on screen

uniform samplerCube skybox;


void main()
{


    fragmentColor = texture(skybox, textureCoords);
}
