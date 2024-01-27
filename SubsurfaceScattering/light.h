#pragma once

#include "algebra.h"

struct Light {
	Vector3 position = {1.0f, 1.0f, 2.0f};
	Vector3 color = {1.0f, 1.0f, 1.0f};
	float ambient = 0.1f, diffuse = 0.5f, specular = 0.5f, m = 15.0f;
};
