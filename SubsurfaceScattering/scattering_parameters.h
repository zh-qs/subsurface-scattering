#pragma once

#include "light.h"
#include "camera.h"

class ScatteringParameters {
  public:
	static constexpr int DEPTH_MAP_SIZE = 1000;
	Light light;
	mutable Camera light_camera;
	int rendered_mesh_idx = 0;
	float wrap = 0.0f;
	float scatter_width = 0.8f;
	float scatter_power = 0.0f;
	Vector3 scatter_color = {1.0f, 1.0f, 1.0f};
	float translucency = 0.0f;
	float sigma_t = 1.0f;
	float grow = 0.0f;
};
