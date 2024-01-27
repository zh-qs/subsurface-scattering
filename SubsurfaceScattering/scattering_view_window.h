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
public:
	ScatteringViewWindow(const ScatteringParameters& parameters);
	virtual void build() override;
};
