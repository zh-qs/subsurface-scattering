#version 410 core

in vec3 world_pos;
in vec3 normal;
in vec2 uv;

out vec4 output_color;

uniform sampler2D color_tex;
uniform sampler2D normal_tex;

// uniform vec4 color;
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

vec3 normalMapping(vec3 norm, vec3 tang, vec3 tn) {
	vec3 bitangent = normalize(cross(norm, tang));
	tang = normalize(cross(bitangent, norm));

	return vec3(tn.x * tang.x + tn.y * bitangent.x + tn.z * norm.x,
				tn.x * tang.y + tn.y * bitangent.y + tn.z * norm.y,
				tn.x * tang.z + tn.y * bitangent.z + tn.z * norm.z);
}

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

	vec2 correct_uv = vec2(uv.x, uv.y);
	vec4 color = texture(color_tex, correct_uv);

	// normal mapping
	vec3 dPdx = dFdx(world_pos);
	vec3 dPdy = dFdy(world_pos);
	vec2 dtdx = dFdx(uv);
	vec2 dtdy = dFdy(uv);
	vec3 tangent = normalize(-dPdx * dtdy.y + dPdy * dtdx.y);

	vec3 tn =
		2.0f * texture(normal_tex, correct_uv).xyz - vec3(1.0f, 1.0f, 1.0f);
	tn.y = -tn.y;

	vec3 disturbed_normal = normalize(normalMapping(normal, tangent, tn));

	float NdotL_wrap = (dot(disturbed_normal, l) + wrap) / (1 + wrap);

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

	//output_color = vec4(trace(), trace(), trace(), 1.0f);
}
