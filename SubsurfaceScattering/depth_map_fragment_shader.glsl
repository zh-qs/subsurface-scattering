#version 410 core

in float distance;

out vec4 output_color;

void main() { output_color = vec4(distance, distance, distance, 1.0f); }