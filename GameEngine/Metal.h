#pragma once
#include "IMaterial.h"

class Metal : public IMaterial
{
public:
	Metal(Vec3F vec, double Fuzz) : albedo(vec), fuzz(Fuzz) {}

	std::tuple<bool, Vec3F, Ray> scatter(Ray const& r, HitRecord const& rec) override
	{
		auto reflected = reflect(MakeUnitVector(r.Direction()), rec.normal);
		auto scattered = Ray(rec.p, reflected + fuzz * RandomPointInUnitSphere());
		return std::make_tuple((dot(scattered.Direction(), rec.normal) > 0), albedo, scattered);
	}
private:
	Vec3F albedo;
	double fuzz;
};