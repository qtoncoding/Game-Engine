#pragma once

#include <array>

#include <limits>	

namespace Detail
{
	double constexpr sqrtNewtonRaphson(double x, double curr, double prev)
	{
		return curr == prev
			? curr
			: sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
	}
}

/*
* Constexpr version of the square root
* Return value:
*	- For a finite and non-negative value of "x", returns an approximation for the square root of "x"
*   - Otherwise, returns NaN
*/
double constexpr const_sqrt(double x)
{
	return x >= 0 && x < std::numeric_limits<double>::infinity()
		? Detail::sqrtNewtonRaphson(x, x, 0)
		: std::numeric_limits<double>::quiet_NaN();
}

template <typename T>
struct Vec3
{
	constexpr Vec3() = default;
	constexpr Vec3(T e1, T e2, T e3) : x(e1), y(e2), z(e3)
	{
	}

	// Positive/Negative
	inline constexpr Vec3<T> const& operator+() const { return *this; }
	inline constexpr Vec3<T> operator-() const { return Vec3<T>(-x, -y, -z); }

	// Arithmetic
	inline constexpr Vec3<T>& operator+=(Vec3<T> const& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	inline constexpr Vec3<T>& operator-=(Vec3<T> const& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	inline constexpr Vec3<T>& operator*=(Vec3<T> const& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;

		return *this;
	}

	inline constexpr Vec3<T>& operator/=(Vec3<T> const& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;

		return *this;
	}

	inline constexpr Vec3<T>& operator*=(T const t)
	{
		x *= t;
		y *= t;
		z *= t;

		return *this;
	}
	
	inline constexpr Vec3<T>& operator/=(T const t)
	{
		x /= t;
		y /= t;
		z /= t;

		return *this;
	}

	// Vector ops
	inline constexpr T SquaredLength() const { return x * x + y * y + z * z; }
	inline constexpr T Length() { return const_sqrt(SquaredLength()); }
	inline void MakeUnitVector() 
	{
		T k = 1.0f / Length();
		x *= k;
		y *= k;
		z *= k;
	}

	T x;
	T y;
	T z;
};

template <typename T>
inline
constexpr Vec3<T> operator+(Vec3<T> const& v1, Vec3<T> const& v2)
{
	return Vec3<T>(v1.x + v2.x,
				   v1.y + v2.y,
				   v1.z + v2.z);
}

template <typename T>
inline
constexpr Vec3<T> operator-(Vec3<T> const& v1, Vec3<T> const& v2)
{
	return Vec3<T>(v1.x - v2.x,
				   v1.y - v2.y,
				   v1.z - v2.z);
}

template <typename T>
inline
constexpr Vec3<T> operator*(Vec3<T> const& v1, Vec3<T> const& v2)
{
	return Vec3<T>(v1.x * v2.x,
				   v1.y * v2.y,
				   v1.z * v2.z);
}

template <typename T>
inline
constexpr Vec3<T> operator/(Vec3<T> const& v1, Vec3<T> const& v2)
{
	return Vec3<T>(v1.x / v2.x,
				   v1.y / v2.y,
				   v1.z / v2.z);
}

template <typename T>
inline
constexpr Vec3<T> operator*(Vec3<T> const& v, T t)
{
	return Vec3<T>(v.x * t,
				   v.y * t,
				   v.z * t);
}

template <typename T>
inline
constexpr Vec3<T> operator*(T t, Vec3<T> const& v)
{
	return v * t;
}

template <typename T>
inline
constexpr Vec3<T> operator/(Vec3<T> const& v, T t)
{
	return Vec3<T>(v.x / t,
				   v.y / t,
				   v.z / t);
}

template <typename T>
inline
constexpr T dot(Vec3<T> const& v1, Vec3<T> const& v2)
{
	return v1.x * v2.x +
		   v1.y * v2.y +
		   v1.z * v2.z;
}

template <typename T>
inline
constexpr Vec3<T> cross(Vec3<T> const& v1, Vec3<T> const& v2)
{
	return Vec3<T>( (v1.y*v2.z - v1.z*v2.y),
				   -(v1.x*v2.z - v1.z*v2.x),
				    (v1.x*v2.y - v1.y*v2.x) );
}

using Vec3F = Vec3<double>;

template <typename T>
inline
constexpr Vec3<T> MakeUnitVector(Vec3<T> vec)
{
	auto k = 1.0f / vec.Length();
	return Vec3<T>(vec.x * k, vec.y * k, vec.z * k );
}

template <typename T>
inline
constexpr Vec3<T> reflect(Vec3<T> const& v, Vec3<T> const& n)
{
	return v - 2 * dot(v, n)*n;
}

