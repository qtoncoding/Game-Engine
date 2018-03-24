#pragma once
#include "IMaterial.h"
#include "Vec3.h"
#include <random>
class Lambertian : public IMaterial
{
public:
	Lambertian(Vec3F const& a) : albedo(a) {}
	std::tuple<bool, Vec3F, Ray> scatter(Ray const&, HitRecord const& rec) override
	{
		auto target = rec.p + rec.normal + RandomPointInUnitSphere();
		return std::make_tuple(true, albedo, Ray(rec.p, target - rec.p));
	}

private:
	Vec3F RandomPointInUnitSphere()
	{
		Vec3F p;
		do
		{
			p = 2.0f * Vec3F(dist(ranDevice), dist(ranDevice), dist(ranDevice)) - Vec3F(1, 1, 1);
		} while (p.SquaredLength() >= 1.0f);
		return p;
	}

	Vec3F albedo;
	std::default_random_engine ranDevice;
	std::uniform_real_distribution<float> dist;

};