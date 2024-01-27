#version 410 core

in vec3 world_pos;
in vec3 normal;
in vec2 uv;

out vec4 output_color;

uniform sampler2D color_tex;
uniform sampler2D normal_tex;
uniform sampler2D diffuse_tex;

uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 cam_pos;

uniform float ambient;
uniform float specular;
uniform float m_exponent;

uniform float diffuse_blur;

vec3 normalMapping(vec3 norm, vec3 tang, vec3 tn) {
	vec3 bitangent = normalize(cross(norm, tang));
	tang = normalize(cross(bitangent, norm));

	return vec3(tn.x * tang.x + tn.y * bitangent.x + tn.z * norm.x,
				  tn.x * tang.y + tn.y * bitangent.y + tn.z * norm.y,
				  tn.x * tang.z + tn.y * bitangent.z + tn.z * norm.z);
}

int texture_pyk(float rx, float ry) {
    vec3 value = texture(diffuse_tex, vec2(uv.x + rx, uv.y + ry)).xyz;
    return value.xyz == vec3(0, 0, 0) ? 0 : 1;
}

void main() {
    float light_dist = length(light_pos - world_pos);
	vec3 l = normalize(light_pos - world_pos);
	vec3 v = normalize(cam_pos - world_pos);
	vec3 r = normalize(reflect(-l, normal));

	vec2 correct_uv = vec2(uv.x, 1.0f - uv.y);
	vec4 color = texture(color_tex, correct_uv);
    vec4 diffuse = texture(diffuse_tex, uv);

    if(diffuse_blur != 0) {
        float r = diffuse_blur;
        diffuse +=
            texture(diffuse_tex, vec2(uv.x + r, uv.y    )) +
            texture(diffuse_tex, vec2(uv.x + r, uv.y - r)) +
            texture(diffuse_tex, vec2(uv.x + r, uv.y + r)) +
            texture(diffuse_tex, vec2(uv.x - r, uv.y    )) +
            texture(diffuse_tex, vec2(uv.x - r, uv.y - r)) +
            texture(diffuse_tex, vec2(uv.x - r, uv.y + r)) +
            texture(diffuse_tex, vec2(uv.x    , uv.y + r)) +
            texture(diffuse_tex, vec2(uv.x    , uv.y - r));
        diffuse /= 1 +
            texture_pyk(+r,  0) +
            texture_pyk(+r, -r) +
            texture_pyk(+r, +r) +
            texture_pyk(-r,  0) +
            texture_pyk(-r, -r) +
            texture_pyk(-r, +r) +
            texture_pyk( 0, +r) +
            texture_pyk( 0, -r);
    }
	
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

	float specular_part = specular * pow(max(dot(r, v), 0), m_exponent);
	if (dot(disturbed_normal, l) <= 0) {
		specular_part = 0;
	}

	output_color = vec4(color.xyz * light_color * ambient + diffuse.xyz +
							light_color * specular_part,
						color.w);
}
