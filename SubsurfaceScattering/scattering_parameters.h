#pragma once

#include "light.h"

class ScatteringParameters {
  public:
	Light light;
	int rendered_mesh_idx = 0;
	float wrap = 0.0f;
	float scatter_width = 0.8f;
	float scatter_power = 0.0f;
	Vector3 scatter_color = {1.0f, 1.0f, 1.0f};
    int scatter_falloff = 2;
    bool angle_scatter = false;
};
