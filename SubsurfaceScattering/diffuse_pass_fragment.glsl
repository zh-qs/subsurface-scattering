#version 410 core

in vec3 world_pos;
in vec3 normal;
in vec2 uv;

out vec4 output_color;

uniform sampler2D color_tex;
uniform sampler2D normal_tex;

uniform vec3 light_pos;
uniform vec3 light_color;

uniform float diffuse;

uniform float wrap;
uniform vec3 scatter_color;
uniform float scatter_width;
uniform float scatter_power;
uniform int angle_scatter;
uniform int scatter_falloff;

vec3 normalMapping(vec3 norm, vec3 tang, vec3 tn) {
	vec3 bitangent = normalize(cross(norm, tang));
	tang = normalize(cross(bitangent, norm));

	return vec3(tn.x * tang.x + tn.y * bitangent.x + tn.z * norm.x,
				  tn.x * tang.y + tn.y * bitangent.y + tn.z * norm.y,
				  tn.x * tang.z + tn.y * bitangent.z + tn.z * norm.z);
}

void main() {
    float light_dist = length(light_pos - world_pos);
	vec3 l = normalize(light_pos - world_pos);

	vec2 correct_uv = vec2(uv.x, 1.0f - uv.y);
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
	}
    else if(angle_scatter == 0) {
        scatter = scatter_width;
    }
    else {
		scatter = smoothstep(0, scatter_width, NdotL_wrap) *
				  smoothstep(scatter_width * 2, scatter_width, NdotL_wrap);
	}

    scatter /= pow(light_dist, scatter_falloff);

	float diffuse_part = diffuse * max(NdotL_wrap, 0);
	vec3 scatter_part = scatter_power * scatter * scatter_color;

	output_color = vec4(color.xyz * light_color * (diffuse_part + scatter_part), 1.0f);
}