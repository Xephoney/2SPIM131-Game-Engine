#version 410 core

in vec4 fragColor;
in vec3 normalTransposed;
in vec2 UV;

out vec4 fragmentColor;

uniform sampler2D diffuse0;

void main()
{
    if(texture(diffuse0, UV).a < 0.05)
        discard;
    fragmentColor = texture(diffuse0, UV);
}
