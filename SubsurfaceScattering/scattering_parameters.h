#pragma once

#include "light.h"

class ScatteringParameters {
  public:
	Light light;
	float wrap = 0.0f;
	float scatter_width = 0.0f;
	Vector3 scatter_color = {1.0f, 1.0f, 1.0f};
};
