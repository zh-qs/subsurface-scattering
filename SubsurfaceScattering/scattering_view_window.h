#pragma once

#include "window.h"
#include "mesh.h"

class ScatteringViewWindow : public Window {
	TriMesh mesh;

	FrameBuffer fbo;
	Camera camera;
	RenderTexture texture;
public:
	ScatteringViewWindow();
	virtual void build() override;
};