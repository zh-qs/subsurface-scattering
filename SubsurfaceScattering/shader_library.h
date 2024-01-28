#pragma once

#include "shader.h"
#include <type_traits>

enum class ShaderType {
	Simple, Axes, Phong, PhongDeformed, DepthMap, Textured, DiffusePass,
};

class ShaderLibrary {
	static constexpr int SHADER_COUNT = 7;
	static Shader shaders[SHADER_COUNT];

	static bool initialized;

public:
	static void init();
	static Shader& get_shader(const ShaderType& type);
	static void destroy_shaders();
};
