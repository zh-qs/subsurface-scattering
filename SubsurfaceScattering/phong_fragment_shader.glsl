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

void main() {
	vec3 l = normalize(light_pos - world_pos);
	vec3 v = normalize(cam_pos - world_pos);
	vec3 r = normalize(reflect(-l, normal));

    float NdotL_wrap = (dot(normal, l) + wrap) / (1 + wrap);

    float scatter =
        smoothstep(0, scatter_width, NdotL_wrap) * 
        smoothstep(scatter_width * 2, scatter_width, NdotL_wrap);

    float diffuse_part = diffuse * max(NdotL_wrap, 0);
    vec3 scatter_part = scatter * scatter_color * light_color;

    float specular_part = specular * pow(max(dot(r, v), 0), m_exponent);
    if(NdotL_wrap <= 0) {
        specular_part = 0;
    }

	output_color =
        vec4(color.xyz * light_color * (ambient + diffuse_part)
             + light_color * specular_part + scatter_part, color.w);
}
