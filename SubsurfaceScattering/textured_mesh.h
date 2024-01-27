#pragma once

#include "mesh.h"

class TexturedTriMesh : public TriMesh {
	VertexBuffer uv_vbo;

	Texture color_texture;
	Texture normal_texture;

	FrameBuffer diffuse_fbo;
	RenderTexture diffuse_texture;

  public:
	TexturedTriMesh() : TriMesh(ShaderType::Textured) {
		color_texture.init();
		color_texture.bind();
		color_texture.configure();
		color_texture.unbind();

		normal_texture.init();
		normal_texture.bind();
		normal_texture.configure();
		normal_texture.unbind();

		diffuse_fbo.init();
		diffuse_fbo.bind();
		diffuse_texture.init();
		diffuse_texture.bind();
		diffuse_texture.configure();
		diffuse_texture.unbind();
		diffuse_fbo.unbind();

		vao.bind();
		uv_vbo.init();
		uv_vbo.bind();
		uv_vbo.attrib_buffer(2, 2);
		vao.unbind();

		Shader &shader = ShaderLibrary::get_shader(shader_type);
		shader.use();
		glUniform1i(shader.get_uniform_location("color_tex"), 0);
		glUniform1i(shader.get_uniform_location("normal_tex"), 1);
		glUniform1i(shader.get_uniform_location("diffuse_tex"), 2);

		Shader &diffuse_shader = ShaderLibrary::get_shader(ShaderType::DiffusePass);
		diffuse_shader.use();
		glUniform1i(diffuse_shader.get_uniform_location("color_tex"), 0);
		glUniform1i(diffuse_shader.get_uniform_location("normal_tex"), 1);
	}

	void set_color_texture(int width, int height, const void *data) {
		color_texture.bind();
		color_texture.set_image(width, height, data);
	}

	void set_normal_texture(int width, int height, const void *data) {
		normal_texture.bind();
		normal_texture.set_image(width, height, data);
	}

	void set_uvs(const std::vector<Vector2> &uvs) {
		uv_vbo.bind();
		uv_vbo.set_static_data(reinterpret_cast<const float *>(uvs.data()),
							   uvs.size() * sizeof(Vector2));
	}

	void render(const Camera &camera, const ScatteringParameters &parameters,
				int width, int height) {
		glActiveTexture(GL_TEXTURE0);
		color_texture.bind();
		glActiveTexture(GL_TEXTURE1);
		normal_texture.bind();
		glActiveTexture(GL_TEXTURE2);
		diffuse_texture.bind();

		auto pv = camera.get_projection_matrix(width, height) *
				  camera.get_view_matrix();

		glEnable(GL_CULL_FACE);

		Shader &shader = ShaderLibrary::get_shader(shader_type);

		shader.use();
		shader.set_pv(pv);
		shader.set_m(model);
		shader.set_color(color.x, color.y, color.z, color.w);
		shader.set_camera_position(camera.get_world_position());
		shader.set_light(parameters.light);

		vao.bind();
		glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, nullptr);
		// glDrawArrays(MODE, 0, point_count);
		vao.unbind();
	}

	void render_diffuse(const Camera &camera,
						const ScatteringParameters &parameters) {

		diffuse_texture.bind();
		diffuse_texture.set_size(color_texture.get_width(),
								 color_texture.get_height());

		diffuse_fbo.bind();
		glViewport(0, 0, color_texture.get_width(), color_texture.get_height());
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		color_texture.bind();
		glActiveTexture(GL_TEXTURE1);
		normal_texture.bind();

		Shader &shader = ShaderLibrary::get_shader(ShaderType::DiffusePass);

		shader.use();
		shader.set_m(model);
		shader.set_light(parameters.light);
		shader.set_wrap(parameters.wrap);
		shader.set_scatter(parameters.scatter_width, parameters.scatter_power,
						   parameters.scatter_color, parameters.scatter_falloff,
						   parameters.angle_scatter);

		vao.bind();
		glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, nullptr);
		// glDrawArrays(MODE, 0, point_count);
		vao.unbind();

		diffuse_fbo.unbind();
        diffuse_texture.unbind();
	}
};
