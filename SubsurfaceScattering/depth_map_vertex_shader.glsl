#version 410 core

layout(location = 0) in vec3 input_pos;
layout(location = 1) in vec3 input_normal;

out float distance;

uniform mat4 pv;
uniform mat4 m;
uniform vec3 light_pos;

void main() {
	float grow = 0.05f;
	vec3 p = input_pos + input_normal * grow;
	vec4 world4 = m * vec4(p, 1.0f);
	distance = length(light_pos - (m * vec4(input_pos, 1.0f)).xyz);
	gl_Position = pv * world4;
}