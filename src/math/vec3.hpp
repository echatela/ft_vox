#pragma once

#include <cassert>
#include <cmath>
#include <ostream>

namespace scm
{

struct Vec3
{
	float x;
	float y;
	float z;

	Vec3()
	    : x(0.0f),
	      y(0.0f),
	      z(0.0f)
	{
	}

	explicit Vec3(float s)
	    : x(s),
	      y(s),
	      z(s)
	{
	}

	Vec3(float x, float y, float z)
	    : x(x),
	      y(y),
	      z(z)
	{
	}

	float& operator[](int i)
	{
		assert(i >= 0 && i < 3);
		return (&x)[i];
	}

	const float& operator[](int i) const
	{
		assert(i >= 0 && i < 2);
		return (&x)[i];
	}

	Vec3& operator+=(const Vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vec3& operator-=(const Vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vec3& operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	Vec3& operator/=(float s)
	{
		assert(s != 0.0f);
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}
};

static_assert(sizeof(Vec3) == 3 * sizeof(float), "Vec3 must be tightly packed");

inline Vec3 operator-(const Vec3& v)
{
	return Vec3(-v.x, -v.y, -v.z);
}

inline Vec3 operator+(const Vec3& a, const Vec3& b)
{
	return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vec3 operator-(const Vec3& a, const Vec3& b)
{
	return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vec3 operator*(const Vec3& a, const Vec3& b)
{
	return Vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

inline Vec3 operator*(const Vec3& v, float s)
{
	return Vec3(v.x * s, v.y * s, v.z * s);
}

inline Vec3 operator*(float s, const Vec3& v)
{
	return Vec3(v.x * s, v.y * s, v.z * s);
}

inline Vec3 operator/(const Vec3& v, float s)
{
	assert(s != 0.0f);
	return Vec3(v.x / s, v.y / s, v.z / s);
}

inline float dot(const Vec3& a, const Vec3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 cross(const Vec3& a, const Vec3& b)
{
	return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
	            a.x * b.y - a.y * b.x);
}

inline float lengthSquared(const Vec3& v)
{
	return dot(v, v);
}

inline float length(const Vec3& v)
{
	return std::sqrt(lengthSquared(v));
}

inline Vec3 normalize(const Vec3& v)
{
	const float len = length(v);

	assert(len > 0.0f);
	return v / len;
}

inline Vec3 min(const Vec3& a, const Vec3& b)
{
	return Vec3(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y,
	            a.z < b.z ? a.z : b.z);
}

inline Vec3 max(const Vec3& a, const Vec3& b)
{
	return Vec3(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y,
	            a.z > b.z ? a.z : b.z);
}

inline const float* valuePtr(const Vec3& v)
{
	return &v.x;
}

inline float* valuePtr(Vec3& v)
{
	return &v.x;
}

inline std::ostream& operator<<(std::ostream& os, const Vec3& v)
{
	return os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
}
} // namespace scm
