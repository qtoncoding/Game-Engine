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
	Vec3F albedo;
};