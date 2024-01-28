#pragma once

#include "window.h"
#include "mesh.h"
#include "textured_mesh.h"
#include "scattering_parameters.h"

class ScatteringViewWindow : public Window {
	const ScatteringParameters &parameters;

    TriMesh light;
	TriMesh mesh;
	TexturedTriMesh salt;
	TexturedTriMesh head;

	FrameBuffer fbo;
	Camera camera;
	RenderTexture texture;

	FrameBuffer depth_map_fbo;
	RenderTexMap depth_map_texture;
	GLint grow_location_dms;

  public:
    RenderTexture diffuse_texture;

	ScatteringViewWindow(const ScatteringParameters& parameters);
	virtual void build() override;
};
