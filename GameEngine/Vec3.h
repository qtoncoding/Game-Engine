#pragma once

#include <array>
#include <cmath>

template <typename T>
class Vec3
{
public:
	Vec3() = default;
	Vec3(T e1, T e2, T e3)
	{
		e = { e1, e2, e3 };
	}
	Vec3(Vec3<T> const& v) = default;
	Vec3(Vec3<T>&& v) = default;
	Vec3<T>& operator=(Vec3<T> const& v) = default;
	Vec3<T>& operator=(Vec3<T>&& v) = default;

	inline T x() const { return e[0]; }
	inline T y() const { return e[1]; }
	inline T z() const { return e[2]; }

	// Positive/Negative
	inline Vec3<T> const& operator+() const { return *this; }
	inline Vec3<T> operator-() const { return Vec3<T>(-e[0], -e[1], -e[2]); }

	// Indexing
	inline T operator[](int i) const { return e[i]; }
	inline T& operator[](int i) { return e[i]; }

	// Arithmetic
	inline Vec3<T>& operator+=(Vec3<T> const& v) 
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];

		return *this;
	}

	inline Vec3<T>& operator-=(Vec3<T> const& v)
	{
		e[0] -= v.e[0];
		e[1] -= v.e[1];
		e[2] -= v.e[2];

		return *this;
	}

	inline Vec3<T>& operator*=(Vec3<T> const& v)
	{
		e[0] *= v.e[0];
		e[1] *= v.e[1];
		e[2] *= v.e[2];

		return *this;
	}

	inline Vec3<T>& operator/=(Vec3<T> const& v)
	{
		e[0] /= v.e[0];
		e[1] /= v.e[1];
		e[2] /= v.e[2];

		return *this;
	}

	inline Vec3<T>& operator*=(T const t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;

		return *this;
	}
	
	inline Vec3<T>& operator/=(T const t)
	{
		e[0] /= t;
		e[1] /= t;
		e[2] /= t;

		return *this;
	}

	// Vector ops
	inline T SquaredLength() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
	inline T Length() const { return std::sqrt(SquaredLength()); }
	inline void MakeUnitVector() 
	{
		T k = 1.0f / Length();
		e[0] *= k;
		e[1] *= k;
		e[2] *= k;
	}

	// Global operators
	template <typename T> friend Vec3<T> operator+(Vec3<T> const& v1, Vec3<T> const& v2);
	template <typename T> friend Vec3<T> operator-(Vec3<T> const& v1, Vec3<T> const& v2);
	template <typename T> friend Vec3<T> operator*(Vec3<T> const& v1, Vec3<T> const& v2);
	template <typename T> friend Vec3<T> operator/(Vec3<T> const& v1, Vec3<T> const& v2);
	template <typename T> friend Vec3<T> operator*(Vec3<T> const& v, T t);
	template <typename T> friend Vec3<T> operator*(T t, Vec3<T> const& v);
	template <typename T> friend Vec3<T> operator/(Vec3<T> const& v, T t);
	template <typename T> friend T dot(Vec3<T> const& v1, Vec3<T> const& v2);
	template <typename T> friend Vec3<T> cross(Vec3<T> const& v1, Vec3<T> const& v2);
	template <typename T> friend Vec3<T> reflect(Vec3<T> const& v1, Vec3<T> const& v2);
private:
	std::array<T, 3> e;
};

template <typename T>
inline
Vec3<T> operator+(Vec3<T> const& v1, Vec3<T> const& v2) 
{
	return Vec3<T>(v1.e[0] + v2.e[0],
				   v1.e[1] + v2.e[1],
				   v1.e[2] + v2.e[2]);
}

template <typename T>
inline
Vec3<T> operator-(Vec3<T> const& v1, Vec3<T> const& v2)
{
	return Vec3<T>(v1.e[0] - v2.e[0],
				   v1.e[1] - v2.e[1],
				   v1.e[2] - v2.e[2]);
}

template <typename T>
inline
Vec3<T> operator*(Vec3<T> const& v1, Vec3<T> const& v2)
{
	return Vec3<T>(v1.e[0] * v2.e[0],
				   v1.e[1] * v2.e[1],
				   v1.e[2] * v2.e[2]);
}

template <typename T>
inline
Vec3<T> operator/(Vec3<T> const& v1, Vec3<T> const& v2)
{
	return Vec3<T>(v1.e[0] / v2.e[0],
				   v1.e[1] / v2.e[1],
				   v1.e[2] / v2.e[2]);
}

template <typename T>
inline
Vec3<T> operator*(Vec3<T> const& v, T t)
{
	return Vec3<T>(v.e[0] * t,
				   v.e[1] * t,
				   v.e[2] * t);
}

template <typename T>
inline
Vec3<T> operator*(T t, Vec3<T> const& v)
{
	return v * t;
}

template <typename T>
inline
Vec3<T> operator/(Vec3<T> const& v, T t)
{
	return Vec3<T>(v.e[0] / t,
				   v.e[1] / t,
				   v.e[2] / t);
}

template <typename T>
inline
T dot(Vec3<T> const& v1, Vec3<T> const& v2)
{
	return v1.e[0] * v2.e[0] +
		   v1.e[1] * v2.e[1] +
		   v1.e[2] * v2.e[2];
}

template <typename T>
inline
Vec3<T> cross(Vec3<T> const& v1, Vec3<T> const& v2)
{
	return Vec3<T>( (v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
				   -(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0]),
				    (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]) );
}

template <typename T>
inline
Vec3<T> MakeUnitVector(Vec3<T> vec)
{
	Vec3F result = vec;
	result.MakeUnitVector();
	return result;
}

template <typename T>
inline
Vec3<T> reflect(Vec3<T> const& v, Vec3<T> const& n)
{
	return v - 2 * dot(v, n)*n;
}

using Vec3F = Vec3<double>;
