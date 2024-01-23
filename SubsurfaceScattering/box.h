#pragma once

#include "algebra.h"

struct Box {
	float x_min, x_max, y_min, y_max, z_min, z_max;

	inline bool contains(const Vector3& v) const { return v.x >= x_min && v.x <= x_max && v.y >= y_min && v.y <= y_max && v.z >= z_min && v.z <= z_max; }
	inline bool contains(float x, float y, float z) const { return x >= x_min && x <= x_max && y >= y_min && y <= y_max && z >= z_min && z <= z_max; }

	inline void add(const Vector3& v) {
		x_min = std::min(v.x, x_min);
		x_max = std::max(v.x, x_max);
		y_min = std::min(v.y, y_min);
		y_max = std::max(v.y, y_max);
		z_min = std::min(v.z, z_min);
		z_max = std::max(v.z, z_max);
	}

	inline void merge_with(const Box& box)
	{
		x_min = std::min(box.x_min, x_min);
		x_max = std::max(box.x_max, x_max);
		y_min = std::min(box.y_min, y_min);
		y_max = std::max(box.y_max, y_max);
		z_min = std::min(box.z_min, z_min);
		z_max = std::max(box.z_max, z_max);
	}

	inline void intersect_with(const Box& box)
	{
		x_min = std::max(box.x_min, x_min);
		x_max = std::min(box.x_max, x_max);
		y_min = std::max(box.y_min, y_min);
		y_max = std::min(box.y_max, y_max);
		z_min = std::max(box.z_min, z_min);
		z_max = std::min(box.z_max, z_max);
	}

	inline bool is_empty() const
	{
		return x_max < x_min
			|| y_max < y_min
			|| z_max < z_min;
	}

	inline void offset_by(const Vector3& off)
	{
		x_min -= off.x;
		x_max += off.x;
		y_min -= off.y;
		y_max += off.y;
		z_min -= off.z;
		z_max += off.z;
	}

	inline void offset_by(const float& off)
	{
		x_min -= off;
		x_max += off;
		y_min -= off;
		y_max += off;
		z_min -= off;
		z_max += off;
	}

	inline Vector3 center() const {
		return { 0.5f * (x_min + x_max),0.5f * (y_min + y_max), 0.5f * (z_min + z_max) };
	}

	inline Vector3 min() const {
		return { x_min,y_min,z_min };
	}

	inline Vector3 max() const {
		return { x_max,y_max,z_max };
	}

	static inline Box infinite() {
		const float inf = INFINITY;
		return { -inf,inf,-inf,inf,-inf,inf };
	}

	static inline Box degenerate() {
		const float inf = INFINITY;
		return { inf,-inf,inf,-inf,inf,-inf };
	}

	static inline Box intersect(const Box& b1, const Box& b2)
	{
		Box box;
		box.x_min = std::max(b1.x_min, b2.x_min);
		box.x_max = std::min(b1.x_max, b2.x_max);
		box.y_min = std::max(b1.y_min, b2.y_min);
		box.y_max = std::min(b1.y_max, b2.y_max);
		box.z_min = std::max(b1.z_min, b2.z_min);
		box.z_max = std::min(b1.z_max, b2.z_max);
		return box;
	}
};

template <class T>
struct RangedBox {
	Box box;
	Range<float> us, vs;
};