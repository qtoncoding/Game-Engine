#pragma once
#include <cmath>
#include <array>

namespace GE
{
template <typename T>
struct Vec2D
{
	inline Vec2D() : x(0), y(0) {}
	inline Vec2D(T _x, T _y) : x(_x), y(_x) {}
	inline Vec2D(Vec2D const& v) : x(v.x), y(v.y) {}
	inline T Magnitude() const { return std::sqrt(x * x + y * y); }
	inline T Magnitude2() const { return x*x + y*y; }
	inline Vec2D Norm() const { T r = 1 / Magnitude(); return Vec2D(x * r, y * r); }
	inline Vec2D Perp() const { return Vec2D(-y, x); }
	inline T Dot(Vec2D const& rhs) { return x * rhs.x + y * rhs.y; }
	inline T Cross(Vec2D const& rhs) { return x * rhs.y - y * rhs.x; }
	inline Vec2D operator+ (Vec2D const& rhs) const { return Vec2D(x + rhs.x, y + rhs.y); }
	inline Vec2D operator- (Vec2D const& rhs) const { return Vec2D(x - rhs.x, y - rhs.y); }
	inline Vec2D operator* (T const& rhs) const { return Vec2D(x * rhs, y * rhs); }
	inline Vec2D operator/ (T const& rhs) const { return Vec2D(x / rhs, y / rhs); }
	inline Vec2D& operator+= (Vec2D const& rhs) { this->x += rhs.x; this->y += rhs.y; return *this; }
	inline Vec2D& operator-= (Vec2D const& rhs) { this->x -= rhs.x; this->y -= rhs.y; return *this; }
	inline Vec2D& operator*= (T const& rhs) { this->x *= rhs; this->y *= rhs; return *this; }
	inline Vec2D& operator/= (T const& rhs) { this->x /= rhs; this->y /= rhs; return *this; }
	inline operator Vec2D<int32_t>() const { return { static_cast<int32_t>(x), static_cast<int32_t>(y) }; }
	inline operator Vec2D<float>() const { return { static_cast<float>(x), static_cast<float>(y) }; }
	inline T& operator[] (std::size_t i) { return data[i]; }

	union
	{
		std::array<T, 2> data{ 0, 0 };
		struct
		{
			T x;
			T y;
		};
	};
};

template <typename T> inline Vec2D<T> operator* (float const& lhs, Vec2D<T> const& rhs) { return Vec2D<T>(lhs * rhs.x, lhs * rhs.y); }
template <typename T> inline Vec2D<T> operator* (double const& lhs, Vec2D<T> const& rhs) { return Vec2D<T>(lhs * rhs.x, lhs * rhs.y); }
template <typename T> inline Vec2D<T> operator* (int const& lhs, Vec2D<T> const& rhs) { return Vec2D<T>(lhs * rhs.x, lhs * rhs.y); }
template <typename T> inline Vec2D<T> operator/ (float const& lhs, Vec2D<T> const& rhs) { return Vec2D<T>(lhs * rhs.x, lhs * rhs.y); }
template <typename T> inline Vec2D<T> operator/ (double const& lhs, Vec2D<T> const& rhs) { return Vec2D<T>(lhs * rhs.x, lhs * rhs.y); }
template <typename T> inline Vec2D<T> operator/ (int const& lhs, Vec2D<T> const& rhs) { return Vec2D<T>(lhs * rhs.x, lhs * rhs.y); }

using vi2d = Vec2D<int32_t>;
using vf2d = Vec2D<float>;
using vd2d = Vec2D<double>;
}