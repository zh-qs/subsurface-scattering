#include "shader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

GLuint Shader::load_shader(const char *filename, GLenum shader_type) {
	GLuint id = glCreateShader(shader_type);

	std::string shader_code;
	std::ifstream ifstr(filename);

	// read code
	if (!ifstr.good()) {
		throw std::runtime_error(std::string("Cannot open shader file: ") +
								 filename);
	}

	std::stringstream sstr;
	sstr << ifstr.rdbuf();
	shader_code = sstr.str();
	ifstr.close();

	// compile code
	const GLchar *const shader_ptr = shader_code.c_str();
	glShaderSource(id, 1, &shader_ptr, NULL);
	glCompileShader(id);

	// check shader
	GLint result = GL_FALSE;
	int info_log_length;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0) {
		std::vector<char> message(info_log_length + 1);
		glGetShaderInfoLog(id, info_log_length, NULL, message.data());
		printf("%s, %s\n", filename, message.data());
		throw std::runtime_error(message.data());
	}

	return id;
}

void Shader::init_uniform_locations() {
	color_location = get_uniform_location("color");
	pv_location = get_uniform_location("pv");
	m_location = get_uniform_location("m");
	cam_pos_location = get_uniform_location("cam_pos");

	light_pos_location = get_uniform_location("light_pos");
	light_color_location = get_uniform_location("light_color");
	ambient_location = get_uniform_location("ambient");
	diffuse_location = get_uniform_location("diffuse");
	specular_location = get_uniform_location("specular");
	m_exponent_location = get_uniform_location("m_exponent");

	wrap_location = get_uniform_location("wrap");
	scatter_color_location = get_uniform_location("scatter_color");
	scatter_width_location = get_uniform_location("scatter_width");
	scatter_power_location = get_uniform_location("scatter_power");
	scatter_falloff_location = get_uniform_location("scatter_falloff");
	angle_scatter_location = get_uniform_location("angle_scatter");

	translucency_location = get_uniform_location("translucency");
	sigma_t_location = get_uniform_location("sigma_t");
	light_pv_location = get_uniform_location("light_pv");
	depth_map_location = get_uniform_location("depth_map");

    diffuse_blur_location = get_uniform_location("diffuse_blur");
}

void Shader::init(const char *vertex_shader_file,
				  const char *fragment_shader_file) {
	auto vertex_shader_id = load_shader(vertex_shader_file, GL_VERTEX_SHADER);
	auto fragment_shader_id =
		load_shader(fragment_shader_file, GL_FRAGMENT_SHADER);

	// link shaders
	id = glCreateProgram();
	glAttachShader(id, vertex_shader_id);
	glAttachShader(id, fragment_shader_id);
	glLinkProgram(id);

	// check program
	GLint result = GL_FALSE;
	int info_log_length;
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0) {
		std::vector<char> message(info_log_length + 1);
		glGetProgramInfoLog(id, info_log_length, NULL, message.data());
		printf("%s\n", message.data());
		throw std::runtime_error(message.data());
	}

	glDetachShader(id, vertex_shader_id);
	glDetachShader(id, fragment_shader_id);

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	init_uniform_locations();
}

void Shader::init(const char *vertex_shader_file,
				  const char *geometry_shader_file,
				  const char *fragment_shader_file) {
	auto vertex_shader_id = load_shader(vertex_shader_file, GL_VERTEX_SHADER);
	auto geometry_shader_id =
		load_shader(geometry_shader_file, GL_GEOMETRY_SHADER);
	auto fragment_shader_id =
		load_shader(fragment_shader_file, GL_FRAGMENT_SHADER);

	// link shaders
	id = glCreateProgram();
	glAttachShader(id, vertex_shader_id);
	glAttachShader(id, geometry_shader_id);
	glAttachShader(id, fragment_shader_id);
	glLinkProgram(id);

	// check program
	GLint result = GL_FALSE;
	int info_log_length;
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0) {
		std::vector<char> message(info_log_length + 1);
		glGetProgramInfoLog(id, info_log_length, NULL, message.data());
		printf("%s\n", message.data());
		throw std::runtime_error(message.data());
	}

	glDetachShader(id, vertex_shader_id);
	glDetachShader(id, geometry_shader_id);
	glDetachShader(id, fragment_shader_id);

	glDeleteShader(vertex_shader_id);
	glDeleteShader(geometry_shader_id);
	glDeleteShader(fragment_shader_id);

	init_uniform_locations();
}

void Shader::init(const char *vertex_shader_file,
				  const char *tess_control_shader_file,
				  const char *tess_eval_shader_file,
				  const char *fragment_shader_file) {
	auto vertex_shader_id = load_shader(vertex_shader_file, GL_VERTEX_SHADER);
	auto tess_control_shader_id =
		load_shader(tess_control_shader_file, GL_TESS_CONTROL_SHADER);
	auto tess_eval_shader_id =
		load_shader(tess_eval_shader_file, GL_TESS_EVALUATION_SHADER);
	auto fragment_shader_id =
		load_shader(fragment_shader_file, GL_FRAGMENT_SHADER);

	// link shaders
	id = glCreateProgram();
	glAttachShader(id, vertex_shader_id);
	glAttachShader(id, tess_control_shader_id);
	glAttachShader(id, tess_eval_shader_id);
	glAttachShader(id, fragment_shader_id);
	glLinkProgram(id);

	// check program
	GLint result = GL_FALSE;
	int info_log_length;
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0) {
		std::vector<char> message(info_log_length + 1);
		glGetProgramInfoLog(id, info_log_length, NULL, message.data());
		printf("%s\n", message.data());
		throw std::runtime_error(message.data());
	}

	glDetachShader(id, vertex_shader_id);
	glDetachShader(id, tess_control_shader_id);
	glDetachShader(id, tess_eval_shader_id);
	glDetachShader(id, fragment_shader_id);

	glDeleteShader(vertex_shader_id);
	glDeleteShader(tess_control_shader_id);
	glDeleteShader(tess_eval_shader_id);
	glDeleteShader(fragment_shader_id);

	init_uniform_locations();
}

void Shader::init(const char *vertex_shader_file,
				  const char *tess_control_shader_file,
				  const char *tess_eval_shader_file,
				  const char *geometry_shader_file,
				  const char *fragment_shader_file) {
	auto vertex_shader_id = load_shader(vertex_shader_file, GL_VERTEX_SHADER);
	auto tess_control_shader_id =
		load_shader(tess_control_shader_file, GL_TESS_CONTROL_SHADER);
	auto tess_eval_shader_id =
		load_shader(tess_eval_shader_file, GL_TESS_EVALUATION_SHADER);
	auto geometry_shader_id =
		load_shader(geometry_shader_file, GL_GEOMETRY_SHADER);
	auto fragment_shader_id =
		load_shader(fragment_shader_file, GL_FRAGMENT_SHADER);

	// link shaders
	id = glCreateProgram();
	glAttachShader(id, vertex_shader_id);
	glAttachShader(id, tess_control_shader_id);
	glAttachShader(id, tess_eval_shader_id);
	glAttachShader(id, geometry_shader_id);
	glAttachShader(id, fragment_shader_id);
	glLinkProgram(id);

	// check program
	GLint result = GL_FALSE;
	int info_log_length;
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0) {
		std::vector<char> message(info_log_length + 1);
		glGetProgramInfoLog(id, info_log_length, NULL, message.data());
		printf("%s\n", message.data());
		throw std::runtime_error(message.data());
	}

	glDetachShader(id, vertex_shader_id);
	glDetachShader(id, tess_control_shader_id);
	glDetachShader(id, tess_eval_shader_id);
	glDetachShader(id, geometry_shader_id);
	glDetachShader(id, fragment_shader_id);

	glDeleteShader(vertex_shader_id);
	glDeleteShader(tess_control_shader_id);
	glDeleteShader(tess_eval_shader_id);
	glDeleteShader(geometry_shader_id);
	glDeleteShader(fragment_shader_id);

	init_uniform_locations();
}

void Shader::use() { glUseProgram(id); }

GLuint Shader::get_uniform_location(const GLchar *name) {
	return glGetUniformLocation(id, name);
}

void Shader::set_color(const float &r, const float &g, const float &b,
					   const float &a) {
	glUniform4f(color_location, r, g, b, a);
}

void Shader::set_pv(const Matrix4x4 &pv) {
	glUniformMatrix4fv(pv_location, 1, GL_FALSE,
					   GLColumnOrderMatrix4x4(pv).elem);
}

void Shader::set_m(const Matrix4x4 &m) {
	glUniformMatrix4fv(m_location, 1, GL_FALSE, GLColumnOrderMatrix4x4(m).elem);
}

void Shader::set_camera_position(const Vector3 &position) {
	glUniform3f(cam_pos_location, position.x, position.y, position.z);
}

void Shader::set_wrap(const float &wrap) { glUniform1f(wrap_location, wrap); }

void Shader::set_scatter(const float &width, const float &power,
						 const Vector3 &color, const int falloff,
						 const bool angle) {
	glUniform1f(scatter_width_location, width);
	glUniform3f(scatter_color_location, color.x, color.y, color.z);
	glUniform1f(scatter_power_location, power);
	glUniform1i(scatter_falloff_location, falloff);
	glUniform1i(angle_scatter_location, angle ? 1 : 0);
}

void Shader::set_translucency(const float &translucency, const float &sigma_t,
							  const Matrix4x4 &light_pv) {
	glUniform1f(translucency_location, translucency);
	glUniform1f(sigma_t_location, sigma_t);
	glUniformMatrix4fv(light_pv_location, 1, GL_FALSE,
					   GLColumnOrderMatrix4x4(light_pv).elem);
	glUniform1i(depth_map_location, 3);
}

void Shader::set_light(const Light &light) {
	glUniform3f(light_pos_location, light.position.x, light.position.y,
				light.position.z);
	glUniform3f(light_color_location, light.color.x, light.color.y,
				light.color.z);
	glUniform1f(ambient_location, light.ambient);
	glUniform1f(diffuse_location, light.diffuse);
	glUniform1f(specular_location, light.specular);
	glUniform1f(m_exponent_location, light.m);
}

void Shader::set_diffuse_blur(float diffuse_blur) {
    glUniform1f(diffuse_blur_location, diffuse_blur);
}

void Shader::dispose() { glDeleteProgram(id); }
