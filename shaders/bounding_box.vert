#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 outColor;

layout(set = 0, binding = 0) uniform GlobalUbo 
{
	mat4 projection;
	mat4 view;
	vec4 ambientLightColor;
	vec3 lightPosition;
	vec4 lightColor;
} ubo;

void main()
{
	// Calculate vertex position in world space
	vec4 positionWorld = vec4(position, 1.0f);
	gl_Position = ubo.projection * ubo.view * vec4(position, 1.0);
	outColor = color;
}

