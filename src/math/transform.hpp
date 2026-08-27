#pragma once

#include "mat4.hpp"
#include <cmath>

namespace scm
{

inline Mat4 scale(const Mat4& m, const Vec3& v)
{
	Mat4 r = m;

	for (int row = 0; row < 4; ++row)
	{
		r.m[0][row] *= v.x;
		r.m[1][row] *= v.y;
		r.m[2][row] *= v.z;
	}
	return r;
}

inline Mat4 translate(const Mat4& m, const Vec3& v)
{
	Mat4 r = m;

	for (int row = 0; row < 4; ++row)
		r.m[3][row] = m.m[0][row] * v.x + m.m[1][row] * v.y +
		              m.m[2][row] * v.z + m.m[3][row];
	return r;
}

inline Mat4 rotateX(const Mat4& m, float angle)
{
	const float c = std::cos(angle);
	const float s = std::sin(angle);

	Mat4 r = m;

	for (int row = 0; row < 4; ++row)
	{
		r.m[1][row] = m.m[1][row] * c + m.m[2][row] * s;
		r.m[2][row] = m.m[1][row] * -s + m.m[2][row] * c;
	}
	return r;
}

inline Mat4 rotateY(const Mat4& m, float angle)
{
	const float c = std::cos(angle);
	const float s = std::sin(angle);

	Mat4 r = m;

	for (int row = 0; row < 4; ++row)
	{
		r.m[2][row] = m.m[2][row] * c + m.m[0][row] * s;
		r.m[0][row] = m.m[2][row] * -s + m.m[0][row] * c;
	}
	return r;
}

inline Mat4 rotateZ(const Mat4& m, float angle)
{
	const float c = std::cos(angle);
	const float s = std::sin(angle);

	Mat4 r = m;

	for (int row = 0; row < 4; ++row)
	{
		r.m[0][row] = m.m[0][row] * c + m.m[1][row] * s;
		r.m[1][row] = m.m[0][row] * -s + m.m[1][row] * c;
	}
	return r;
}

inline Mat4 rotate(const Mat4& m, float angle, const Vec3& axis)
{
	const float c = std::cos(angle);
	const float s = std::sin(angle);
	const Vec3  a = normalize(axis);
	const Vec3  t = a * (1.0f - c);

	float rot[3][3];

	rot[0][0] = t.x * a.x + c;
	rot[0][1] = t.x * a.y + s * a.z;
	rot[0][2] = t.x * a.z - s * a.y;

	rot[1][0] = t.y * a.x - s * a.z;
	rot[1][1] = t.y * a.y + c;
	rot[1][2] = t.y * a.z + s * a.x;

	rot[2][0] = t.z * a.x + s * a.y;
	rot[2][1] = t.z * a.y - s * a.x;
	rot[2][2] = t.z * a.z + c;

	Mat4 r = m;

	for (int row = 0; row < 4; ++row)
		for (int col = 0; col < 3; ++col)
			r.m[col][row] = m.m[0][row] * rot[col][0] +
			                m.m[1][row] * rot[col][1] +
			                m.m[2][row] * rot[col][2];
	return r;
}

inline float radians(float deg)
{
	return M_PI / 180 * deg;
}

inline float degrees(float rad)
{
	return 180 / M_PI * rad;
}

} // namespace scm
