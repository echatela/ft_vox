#pragma once

#include <cassert>
#include <cmath>
#include <ostream>

#include "vec3.hpp"

namespace scm
{

struct Vec4
{
	float x;
	float y;
	float z;
	float w;

	Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	explicit Vec4(float s) : x(s), y(s), z(s), w(s) {}
	Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	Vec4(const Vec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

	Vec3 xyz() const
	{
		return Vec3(x, y, z);
	}

	float& operator[](int i)
	{
		assert(i >= 0 && i < 4);
		return (&x)[i];
	}

	const float& operator[](int i) const
	{
		assert(i >= 0 && i < 4);
		return (&x)[i];
	}

	Vec4& operator+=(const Vec4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	Vec4& operator-=(const Vec4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	Vec4& operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}

	Vec4& operator/=(float s)
	{
		assert(s != 0.0f);
		x /= s;
		y /= s;
		z /= s;
		w /= s;
		return *this;
	}
};

static_assert(sizeof(Vec4) == 4 * sizeof(float), "Vec4 must be tightly packed");

inline Vec4 operator-(const Vec4& v)
{
	return Vec4(-v.x, -v.y, -v.z, -v.w);
}

inline Vec4 operator+(const Vec4& a, const Vec4& b)
{
	return Vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

inline Vec4 operator-(const Vec4& a, const Vec4& b)
{
	return Vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

inline Vec4 operator*(const Vec4& a, const Vec4& b)
{
	return Vec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

inline Vec4 operator*(const Vec4& v, float s)
{
	return Vec4(v.x * s, v.y * s, v.z * s, v.w * s);
}

inline Vec4 operator*(float s, const Vec4& v)
{
	return v * s;
}

inline Vec4 operator/(const Vec4& v, float s)
{
	assert(s != 0.0f);
	return Vec4(v.x / s, v.y / s, v.z / s, v.w / s);
}

inline float dot(const Vec4& a, const Vec4& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline float lengthSquared(const Vec4& v)
{
	return dot(v, v);
}

inline float length(const Vec4& v)
{
	return std::sqrt(lengthSquared(v));
}

inline Vec4 normalize(const Vec4& v)
{
	const float len = length(v);

	assert(len > 0.0f);
	return v / len;
}

inline Vec3 perspectiveDivide(const Vec4& v)
{
	assert(v.w != 0.0f);
	return Vec3(v.x / v.w, v.y / v.w, v.z / v.w);
}

inline const float* valuePtr(const Vec4& v)
{
	return &v.x;
}

inline float* valuePtr(Vec4& v)
{
	return &v.x;
}

inline std::ostream& operator<<(std::ostream& os, const Vec4& v)
{
	return os << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')';
}

} // namespace scm
