#type vertex
#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec2 a_UV;

uniform mat4 u_view_projection;
uniform mat4 u_model;
uniform mat4 u_biasedLight;


out vec3 v_fragPos;
out vec4 v_color;
out vec3 normalTransposed;
out vec4 ShadowCoord;

void main()
{
	vec4 worldpos = u_model * vec4(a_Position, 1);
	gl_Position = u_view_projection * worldpos;
	ShadowCoord = u_biasedLight * worldpos;
	v_fragPos = worldpos.xyz;
	normalTransposed = vec3(transpose(inverse(u_model)) * vec4(a_Normal, 0.0));
	v_color = a_Color;
	//v_color = vec4(normalTransposed, 1);
}

#type fragment
#version 450 core
layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out int index;

in vec3 v_fragPos;
in vec4 v_color;
in vec3 normalTransposed;
in vec4 ShadowCoord;

uniform int u_ID = -1;
uniform vec4 u_Color = vec4( 1.0, 1.0, 1.0, 1.0 );
uniform int specularExponent = 128;
uniform float specularStrength = 0.9;
uniform float ambientStrength = 0.45;
uniform vec3 lightDirection = vec3(0, 1, 1);
uniform vec3 lightColor = vec3(1, 1, 1);

uniform vec3 cameraPosition;

uniform sampler2D shadowMap;


vec3 directionalLight()
{
	vec3 Normal = normalize(normalTransposed);
	vec3 lightDir = normalize(lightDirection);
	float diffuse = max(dot(Normal, lightDir), 0);
	
	float specular = 0.0;
	if (diffuse > 0)
	{
		vec3 viewDirection = normalize(cameraPosition - v_fragPos);
		vec3 reflectDirection = reflect(-lightDir, Normal);

		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		float specAmount = pow(max(dot(Normal, halfwayVec), 0.0), specularExponent);
		specular = specAmount * 0.2;
	}

	float shadow = 0.f;
	vec3 lightCoords = ShadowCoord.xyz / ShadowCoord.w;
	if (lightCoords.z <= 1.0f)
	{
		lightCoords = (lightCoords + 1.0f) / 2.0f;

		float closestDepth = texture(shadowMap, lightCoords.xy).r;
		float currentDepth = lightCoords.z;
		float bias = 0.005f;
		if (currentDepth > closestDepth + bias)
			shadow = 1.f;
	}

	vec3 result = lightColor * ( (diffuse * (1.0f - shadow)) + ambientStrength + (specular * (1.0f - shadow)));
	return clamp(result, 0, 1);

}

void main()
{
	
	
	fragmentColor = vec4(directionalLight() , 1.0) * v_color  * u_Color;
	index = u_ID;
}