#pragma once
#include "algebra.h"
#include "light.h"
#include <glad/glad.h>

class Shader {
  protected:
	GLint color_location;
	GLint pv_location;
	GLint m_location;
	GLint cam_pos_location;

	GLint light_pos_location;
	GLint light_color_location;
	GLint ambient_location;
	GLint diffuse_location;
	GLint specular_location;
	GLint m_exponent_location;

	GLint wrap_location;
	GLint scatter_width_location;
	GLint scatter_color_location;
	GLint scatter_power_location;
	GLint scatter_falloff_location;
	GLint angle_scatter_location;

	GLuint load_shader(const char *filename, GLenum shader_type);
	void init_uniform_locations();

  public:
	GLuint id;
	void init(const char *vertex_shader_file, const char *fragment_shader_file);
	void init(const char *vertex_shader_file, const char *geometry_shader_file,
			  const char *fragment_shader_file);
	void init(const char *vertex_shader_file,
			  const char *tess_control_shader_file,
			  const char *tess_eval_shader_file,
			  const char *fragment_shader_file);
	void init(const char *vertex_shader_file,
			  const char *tess_control_shader_file,
			  const char *tess_eval_shader_file,
			  const char *geometry_shader_file,
			  const char *fragment_shader_file);
	void use();
	GLuint get_uniform_location(const GLchar *name);
	void set_color(const float &r, const float &g, const float &b,
				   const float &a);
	void set_pv(const Matrix4x4 &pv);
	void set_m(const Matrix4x4 &m);
	void set_camera_position(const Vector3 &position);
	void set_light(const Light &light);
	void set_wrap(const float &wrap);
	void set_scatter(const float &width, const float &power,
					 const Vector3 &color, const int falloff,
					 const bool angle);

	void dispose();
};
