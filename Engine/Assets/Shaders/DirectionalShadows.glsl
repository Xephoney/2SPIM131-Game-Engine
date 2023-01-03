#type vertex
#version 450 core
layout(location = 0) in vec3 a_Position;

uniform mat4 lightSpaceMatrix;
uniform mat4 u_model;

void main()
{
	gl_Position = lightSpaceMatrix * u_model * vec4(a_Position, 1.0);
}


#type fragment
#version 450 core

//layout(location = 0) out float fragmentDepth;
//
//float near = -0.01;
//float far = 100.0;
//
//float linearizeDepth(float depth)
//{
//	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
//}

void main() 
{
	//fragmentDepth = linearizeDepth(gl_FragCoord.z) / far;
}
