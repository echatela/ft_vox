#pragma once

// Conventions :
//  - column-major storage, indexation m[column][line]
//  - write to left composition : T * R * S apply S first
//  - write hand marker, NDC depth [-1, 1]
//  - radians angles

#include <cassert>
#include <ostream>

#include "vec4.hpp"

namespace scm
{

struct Mat4
{
	union
	{
		float m[4][4];
		float data[16];
	};

	Mat4() : data{} {}

	static Mat4 identity()
	{
		Mat4 r;

		r.m[0][0] = 1.0f;
		r.m[1][1] = 1.0f;
		r.m[2][2] = 1.0f;
		r.m[3][3] = 1.0f;
		return r;
	}

	float* operator[](int col)
	{
		assert(col >= 0 && col < 4);
		return m[col];
	}

	const float* operator[](int col) const
	{
		assert(col >= 0 && col < 4);
		return m[col];
	}

	Vec4 column(int col) const
	{
		assert(col >= 0 && col < 4);
		return Vec4(m[col][0], m[col][1], m[col][2], m[col][3]);
	}

	Vec4 row(int r) const
	{
		assert(r >= 0 && r < 4);
		return Vec4(m[0][r], m[1][r], m[2][r], m[3][r]);
	}
};

static_assert(sizeof(Mat4) == 16 * sizeof(float),
              "Mat4 must be tightly packed");

inline Vec4 operator*(const Mat4& a, const Vec4& v)
{
	Vec4 r;

	for (int row = 0; row < 4; ++row)
		r[row] = a.m[0][row] * v.x
		       + a.m[1][row] * v.y
		       + a.m[2][row] * v.z
		       + a.m[3][row] * v.w;
	return r;
}

inline Mat4 operator*(const Mat4& a, const Mat4& b)
{
	Mat4 r;

	for (int col = 0; col < 4; ++col)
	{
		for (int row = 0; row < 4; ++row)
		{
			float sum = 0.0f;

			for (int k = 0; k < 4; ++k)
				sum += a.m[k][row] * b.m[col][k];
			r.m[col][row] = sum;
		}
	}
	return r;
}

inline Mat4 transpose(const Mat4& a)
{
	Mat4 r;

	for (int col = 0; col < 4; ++col)
		for (int row = 0; row < 4; ++ row)
			r.m[col][row] = a.m[row][col];
	return r;
}

inline const float* valuePtr(const Mat4& a)
{
	return a.data;
}

inline float* valuePtr(Mat4& a)
{
	return a.data;
}

inline std::ostream& operator<<(std::ostream& os, const Mat4& a)
{
	for (int row = 0; row < 4; ++row)
	{
		os << '[';
		for (int col = 0; col < 4; ++col)
			os << (col ? ", " : "") << a.m[col][row];
		os << "]\n";
	}
	return os;
}

} // namespace scm
