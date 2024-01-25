#version 410 core

layout(location = 0) in vec3 input_pos;
layout(location = 1) in vec3 input_normal;
layout(location = 2) in vec2 input_uv;

out vec3 world_pos;
out vec3 normal;
out vec2 uv;

uniform mat4 pv;
uniform mat4 m;

void main() {
	vec4 world4 = m * vec4(input_pos, 1.0f);
	world_pos = world4.xyz;
	normal = normalize((m * vec4(input_normal, 0.0f)).xyz);
	uv = input_uv;
	gl_Position = pv * world4;
}