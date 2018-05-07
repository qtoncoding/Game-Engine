#pragma once
#include "Ray.h"
#include "Vec3.h"
#include "IRayHitable.h"
#include <tuple>
class IMaterial
{
public:
	virtual std::tuple<bool, Vec3F, Ray> scatter(Ray const& r, HitRecord const& rec) = 0;

protected:
	Vec3F RandomPointInUnitSphere()
	{
		Vec3F p;
		do
		{
			p = 2.0 * Vec3F(dist(ranDevice), dist(ranDevice), dist(ranDevice)) - Vec3F(1, 1, 1);
		} while (p.SquaredLength() >= 1.0f);
		return p;
	}

	std::default_random_engine ranDevice;
	std::uniform_real_distribution<double> dist;
};