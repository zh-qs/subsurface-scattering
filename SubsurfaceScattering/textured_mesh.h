#pragma once

#include "mesh.h"

class TexturedTriMesh : public TriMesh {
	VertexBuffer uv_vbo;

	Texture color_texture;
	Texture normal_texture;

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

		vao.bind();
		uv_vbo.init();
		uv_vbo.bind();
		uv_vbo.attrib_buffer(2, 2);
		vao.unbind();

		Shader &shader = ShaderLibrary::get_shader(shader_type);
		shader.use();
		glUniform1i(shader.get_uniform_location("color_tex"), 0);
		glUniform1i(shader.get_uniform_location("normal_tex"), 1);
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
				int width, int height) override {
		glActiveTexture(GL_TEXTURE0);
		color_texture.bind();
		glActiveTexture(GL_TEXTURE1);
		normal_texture.bind();
		TriMesh::render(camera, parameters, width, height);
	}
};