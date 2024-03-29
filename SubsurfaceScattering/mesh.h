#pragma once

#include "algebra.h"
#include "box.h"
#include "buffer.h"
#include "camera.h"
#include "frame_buffer.h"
#include "light.h"
#include "quaternion.h"
#include "scattering_parameters.h"
#include "shader_library.h"
#include "texture.h"
#include "vertex_array.h"
#include <vector>

template <GLenum MODE> class Mesh {
  protected:
	VertexArray vao;
	VertexBuffer vbo;
	VertexBuffer normal_vbo;
	ElementBuffer ebo;
	size_t indices_count = 0;
	bool has_normals = false;

	Box bounding_box;
	void calculate_bounding_box(const std::vector<Vector3> &vertices) {
		bounding_box = Box::degenerate();
		for (const auto &v : vertices)
			bounding_box.add(v);
	}

  public:
	Matrix4x4 model = Matrix4x4::identity();
	Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	ShaderType shader_type;
	bool visible = true;

	explicit Mesh(const ShaderType type = ShaderType::Simple)
		: vao(), vbo(), shader_type(type) {
		vao.init();
		vao.bind();
		vbo.init();
		vbo.bind();
		/*glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);*/
		vbo.attrib_buffer(0, 3);
		if (type == ShaderType::Phong || type == ShaderType::PhongDeformed ||
			type == ShaderType::Textured) {
			has_normals = true;
			normal_vbo.init();
			normal_vbo.bind();
			normal_vbo.attrib_buffer(1, 3);
			/*glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);*/
		}
		ebo.init();
		ebo.bind();
		vao.unbind();

		/*switch (type)
		{
		case ShaderType::Simple:
			shader.init("simple_vertex_shader.glsl",
		"simple_fragment_shader.glsl"); break; case ShaderType::Axes:
			shader.init("axes_vertex_shader.glsl", "axes_fragment_shader.glsl");
			break;
		case ShaderType::Phong:
			shader.init("phong_vertex_shader.glsl",
		"phong_fragment_shader.glsl"); break; case ShaderType::PhongDeformed:
			shader.init("phong_deformed_vertex_shader.glsl",
		"phong_fragment_shader.glsl");
		}*/
	}

	~Mesh() {
		ebo.dispose();
		vbo.dispose();
		normal_vbo.dispose();
		vao.dispose();
	}

	template <class I>
	void set_data(const std::vector<Vector3> &points,
				  const std::vector<I> &indices) {
		calculate_bounding_box(points);

		vbo.bind();
		vbo.set_static_data(reinterpret_cast<const float *>(points.data()),
							points.size() * sizeof(Vector3));

		ebo.bind();
		ebo.set_static_data(
			reinterpret_cast<const unsigned int *>(indices.data()),
			indices.size() * sizeof(I));
		indices_count = indices.size() * sizeof(I) / sizeof(unsigned int);
	}
	void set_data(const std::vector<Vector3> &points);
	void set_normals(const std::vector<Vector3> &normals);
	virtual void render(const Camera &camera,
						const ScatteringParameters &parameters, int width,
						int height);
	void render_with_other_shader(const Camera &camera,
								  const ScatteringParameters &parameters,
								  int width, int height,
								  ShaderType other_shader);
	void render_simple(const Camera &camera, int width, int height);

	const Box &get_bounding_box() const { return bounding_box; }
};

template <GLenum MODE>
void Mesh<MODE>::set_data(const std::vector<Vector3> &points) {
	std::vector<unsigned int> indices(points.size());
	for (int i = 0; i < indices.size(); ++i)
		indices[i] = i;
	set_data(points, indices);
}

template <GLenum MODE>
void Mesh<MODE>::set_normals(const std::vector<Vector3> &normals) {
	if (!has_normals)
		return;

	normal_vbo.bind();
	normal_vbo.set_static_data(reinterpret_cast<const float *>(normals.data()),
							   normals.size() * sizeof(Vector3));
}

template <GLenum MODE>
void Mesh<MODE>::render(const Camera &camera,
						const ScatteringParameters &parameters, int width,
						int height) {
	if (!visible)
		return;

	auto pv =
		camera.get_projection_matrix(width, height) * camera.get_view_matrix();

	glEnable(GL_CULL_FACE);

	Shader &shader = ShaderLibrary::get_shader(shader_type);

	shader.use();
	shader.set_pv(pv);
	shader.set_m(model);
	shader.set_color(color.x, color.y, color.z, color.w);
	shader.set_camera_position(camera.get_world_position());
	shader.set_light(parameters.light);
	shader.set_wrap(parameters.wrap);
	shader.set_scatter(parameters.scatter_width, parameters.scatter_power,
		parameters.scatter_color, parameters.scatter_falloff,
		parameters.angle_scatter);
	shader.set_translucency(parameters.translucency, parameters.sigma_t,
		parameters.light_camera.get_projection_matrix(
			ScatteringParameters::DEPTH_MAP_SIZE,
			ScatteringParameters::DEPTH_MAP_SIZE) *
		parameters.light_camera.get_view_matrix());

	vao.bind();
	glDrawElements(MODE, indices_count, GL_UNSIGNED_INT, nullptr);
	// glDrawArrays(MODE, 0, point_count);
	vao.unbind();
}

template <GLenum MODE>
inline void Mesh<MODE>::render_with_other_shader(
	const Camera &camera, const ScatteringParameters &parameters, int width,
	int height, ShaderType other_shader) {
	const auto old_shader = shader_type;
	shader_type = other_shader;
	render(camera, parameters, width, height);
	shader_type = old_shader;
}

template <GLenum MODE>
void Mesh<MODE>::render_simple(const Camera &camera, int width, int height) {
	if (!visible)
		return;

	auto pv =
		camera.get_projection_matrix(width, height) * camera.get_view_matrix();

	glEnable(GL_CULL_FACE);

	Shader &shader = ShaderLibrary::get_shader(shader_type);

	shader.use();
	shader.set_pv(pv);
	shader.set_m(model);
	shader.set_color(color.x, color.y, color.z, 1.0f);
	shader.set_camera_position(camera.get_world_position());

	vao.bind();
	glDrawElements(MODE, indices_count, GL_UNSIGNED_INT, nullptr);
	// glDrawArrays(MODE, 0, point_count);
	vao.unbind();
}

using LineMesh = Mesh<GL_LINES>;
using LineStripMesh = Mesh<GL_LINE_STRIP>;
using TriMesh = Mesh<GL_TRIANGLES>;
