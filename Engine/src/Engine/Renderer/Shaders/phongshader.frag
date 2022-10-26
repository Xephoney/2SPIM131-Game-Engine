#version 410 core

in vec3 fragPos;
in vec4 color;
in vec3 normalTransposed;              //color received from the pipeline (and vertex shader)
in vec2 UV;

out vec4 fragmentColor;     //color sent to fragment on screen

uniform int specularExponent = 128;
uniform float specularStrength = 0.9;
uniform float lightStrength = 0.1;
uniform float ambientStrength = 0.5;
uniform vec3 lightPosition = vec3(0, 20, 0);
uniform vec3 lightColor = vec3(1, 1, 1.0);

uniform vec3 cameraPosition;

vec3 pointLight()
{
    //ambient
    vec3 ambient = ambientStrength * vec3(color);

    //diffuse
    vec3 lightDirection = normalize(lightPosition - fragPos);
    float angleFactor = max(dot(normalTransposed, lightDirection),1);
    vec3 diffuse = angleFactor * vec3(color) * lightColor * lightStrength;

    //Specular
    vec3 viewDirection = normalize(cameraPosition - fragPos);
    float spec = 0.0;
    if(angleFactor > 0.0)
    {
        vec3 reflectDirection = reflect(-lightDirection, normalTransposed);
        spec = pow(max(dot(viewDirection, reflectDirection), 0.0), specularExponent);
    }
    vec3 specular = spec * lightColor * specularStrength;

    return ambient + diffuse + specular;
}
void main()
{

    vec3 result = pointLight();
    fragmentColor = vec4(result,1.0) *  color;
}
