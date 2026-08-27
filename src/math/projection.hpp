#pragma once

#include "math/mat4.hpp"

namespace scm
{
inline Mat4 perspective(float fovY, float aspectRatio, float front, float back)
{
	float tangent = tan(fovY / 2);
	float top = front * tangent;
	float right = top * aspectRatio;

	Mat4 r;
	r.data[0] = front / right;
	r.data[5] = front / top;
	r.data[10] = -(back + front) / (back - front);
	r.data[11] = -1;
	r.data[14] = -(2 * back * front) / (back - front);
	r.data[15] = 0;
	return r;
}
} // namespace scm
