#version 410 core

in vec3 world_pos;
in vec3 normal;

out vec4 output_color;

uniform vec4 color;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 cam_pos;

uniform float ambient;
uniform float diffuse;
uniform float specular;
uniform float m_exponent;

void main() {
	vec3 l = normalize(light_pos - world_pos);
	vec3 v = normalize(cam_pos - world_pos);
	vec3 r = normalize(reflect(-l, normal));
	output_color =
		vec4(color.xyz * light_color * (ambient + diffuse * max(dot(normal, l), 0)) +
				 specular * light_color * pow(max(dot(r, v), 0), m_exponent),
			 color.w);
}