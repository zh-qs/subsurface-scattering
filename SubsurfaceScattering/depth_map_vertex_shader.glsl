#version 410 core

layout(location = 0) in vec3 input_pos;
layout(location = 1) in vec3 input_normal;

out float distance;

uniform mat4 pv;
uniform mat4 m;
uniform vec3 light_pos;

uniform float grow;

void main() {
	vec4 world4 = m * vec4(input_pos, 1.0f);
	vec4 p = world4 + vec4(normalize(input_normal) * grow, 0.0f);
	distance = length(light_pos - world4.xyz);
	gl_Position = pv * p;
}