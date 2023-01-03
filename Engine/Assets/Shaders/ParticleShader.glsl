#type vertex
#version 450 core

layout(location = 0) in vec4 positionIn;    // 1st attribute buffer = vertex positions
layout(location = 1) in vec3 normalIn;       // 2nd attribute buffer = colors
layout(location = 2) in vec2 uvIn;       // 2nd attribute buffer = colors
layout(location = 3) in vec3 colorIn;       // 2nd attribute buffer = colors


out vec4 fragColor;
out vec3 normalTransposed;
out vec2 UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 worldPos = model * positionIn;
	normalTransposed = vec3(transpose(inverse(model)) * vec4(normalIn, 0.0));
	gl_Position = projection * view * worldPos;
	fragColor = vec4(colorIn, 1.0);
	UV = uvIn;
}

#type fragment
#version 450 core

in vec4 fragColor;
in vec3 normalTransposed;
in vec2 UV;

out vec4 fragmentColor;

uniform sampler2D diffuse0;

void main()
{
	if (texture(diffuse0, UV).a < 0.05)
		discard;
	fragmentColor = texture(diffuse0, UV);
}
