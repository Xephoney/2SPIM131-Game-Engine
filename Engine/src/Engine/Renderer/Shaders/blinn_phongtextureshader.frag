#version 410 core
//Lagd ved hjelp av en video fra Viktor
//link : https://www.youtube.com/watch?v=ZbszezwNSZU

// Det er den samme som den andre blinn_phong shaderen, men jeg la til teksturer her.
in vec3 fragPos;
in vec4 color;
in vec3 normalTransposed;              //color received from the pipeline (and vertex shader)
in vec2 UV;

out vec4 fragmentColor;     //color sent to fragment on screen

uniform int specularExponent = 128;
uniform float specularStrength = 0.9;
uniform float ambientStrength = 0.45;
uniform vec3 lightPosition = vec3(0, 20, 0);
uniform vec3 lightColor = vec3(1, 1, 1);

uniform vec3 cameraPosition;
uniform sampler2D diffuse0;

float specularLight = 0.2;

vec3 pointLight()
{
    vec3 lightVec = lightPosition - fragPos;
    float dist = length(lightVec);
    float a = 0.00001;
    float b = 0.000001;
    float intensity = 1.0 / ( a * dist * dist + b * dist + 1.0);


    //diffuse
    vec3 lightDirection = normalize(lightVec);
    vec3 Normal = normalize(normalTransposed);
    float diffuse = max(dot(Normal, lightDirection), 0);

    //Specular
    float specular = 0.0;
    if(diffuse > 0)
    {
        vec3 viewDirection = normalize(cameraPosition - fragPos);
        vec3 reflectDirection = reflect(-lightDirection, Normal);

        vec3 halfwayVec = normalize(viewDirection + lightDirection);
        float specAmount = pow(max(dot(Normal, halfwayVec), 0.0), specularExponent);
        //float specAmount = pow(max(dot(reflectDirection, viewDirection), 0.0), 1);
        specular = specAmount * specularLight;
    }

    vec3 result = vec3(texture(diffuse0, UV)) * ((diffuse* intensity) + ambientStrength + (specular * intensity));
    return clamp(result,0,1);
}
void main()
{

    vec3 result = pointLight() * lightColor;
    fragmentColor = vec4(result,1.0);
    //fragmentColor = vec4(normalizedNormal,1);
}
