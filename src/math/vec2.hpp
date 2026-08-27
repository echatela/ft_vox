#pragma once

#include <cassert>
#include <cmath>
#include <ostream>

namespace scm
{

struct Vec2
{
	float x;
	float y;

	Vec2() : x(0.0f), y(0.0f) {}
	explicit Vec2(float s) : x(s), y(s) {}
	Vec2(float x, float y) : x(x), y(y) {}

	float& operator[](int i)
	{
		assert(i >= 0 && i < 2);
		return (&x)[i];
	}

	const float& operator[](int i) const
	{
		assert(i >= 0 && i < 2);
		return (&x)[i];
	}

	Vec2& operator+=(const Vec2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vec2& operator-=(const Vec2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vec2& operator*=(float s)
	{
		x *= s;
		y *= s;
		return *this;
	}

	Vec2& operator/=(float s)
	{
		assert(s != 0.0f);
		x /= s;
		y /= s;
		return *this;
	}

};

static_assert(sizeof(Vec2) == 2 * sizeof(float), "Vec2 must be tightly packed");

inline Vec2 operator-(const Vec2& v)
{
	return Vec2(-v.x, -v.y);
}

inline Vec2 operator+(const Vec2& a, const Vec2& b)
{
	return Vec2(a.x + b.x, a.y + b.y);
}

inline Vec2 operator-(const Vec2& a, const Vec2& b)
{
	return Vec2(a.x - b.x, a.y - b.y);
}

inline Vec2 operator*(const Vec2& a, const Vec2& b)
{
	return Vec2(a.x * b.x, a.y * b.y);
}

inline Vec2 operator*(const Vec2& v, float s)
{
	return Vec2(v.x * s, v.y * s);
}

inline Vec2 operator*(float s, const Vec2& v)
{
	return Vec2(v.x * s, v.y * s);
}

inline Vec2 operator/(const Vec2& v, float s)
{
	assert(s != 0.0f);
	return Vec2(v.x / s, v.y / s);
}

inline float dot(const Vec2& a, const Vec2& b)
{
	return a.x * b.x + a.y * b.y;
}

inline float lengthSquared(const Vec2& v)
{
	return dot(v, v);
}

inline float length(const Vec2& v)
{
	return std::sqrt(lengthSquared(v));
}

inline Vec2 normalize(const Vec2& v)
{
	const float len = length(v);

	assert(len > 0.0f);
	return v / len;
}

inline Vec2 min(const Vec2& a, const Vec2& b)
{
	return Vec2(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y);
}

inline Vec2 max(const Vec2& a, const Vec2& b)
{
	return Vec2(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y);
}

inline const float* valuePtr(const Vec2& v)
{
	return &v.x;
}

inline float* valuePtr(Vec2& v)
{
	return &v.x;
}

inline std::ostream &operator<<(std::ostream& os, const Vec2& v)
{
	return os << '(' << v.x << ", " << v.y << ')';
}

}
