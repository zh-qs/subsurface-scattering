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

uniform float wrap;
uniform vec3 scatter_color;
uniform float scatter_width;
uniform float scatter_power;

uniform float translucency;
uniform float sigma_t;
uniform mat4 light_pv;
uniform sampler2D depth_map;

float trace() {
	vec4 tex_coord = light_pv * vec4(world_pos, 1.0f);
	float d_i = texture(depth_map, 0.5f*tex_coord.xy/tex_coord.w - vec2(0.5f,0.5f)).x;
	float d_o = length(light_pos - world_pos);
	return d_o - d_i;
}

void main() {
	float light_dist = length(light_pos - world_pos);
	vec3 l = normalize(light_pos - world_pos);
	vec3 v = normalize(cam_pos - world_pos);
	vec3 r = normalize(reflect(-l, normal));

	float NdotL_wrap = (dot(normal, l) + wrap) / (1 + wrap);

	float scatter;
	if (scatter_width == 0) {
		scatter = 0;
	} else {
		scatter = smoothstep(0, scatter_width, NdotL_wrap) *
				  smoothstep(scatter_width * 2, scatter_width, NdotL_wrap) /
				  light_dist / light_dist;
	}

	float diffuse_part = diffuse * max(NdotL_wrap, 0);
	vec3 scatter_part = scatter_power * scatter * scatter_color;

	vec3 translucent_part =
		translucency * exp(-trace() * sigma_t) * scatter_color;

	float specular_part = specular * pow(max(dot(r, v), 0), m_exponent);
	if (NdotL_wrap <= 0) {
		specular_part = 0;
	}

	output_color =
		vec4(color.xyz * light_color * (ambient + diffuse_part + scatter_part + translucent_part) +
				 light_color * specular_part,
			 color.w);
}
