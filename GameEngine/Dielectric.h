#pragma once
#include "IMaterial.h"
#include <optional>
#include <random>

class Dielectric : public IMaterial
{
public:
	Dielectric(double RefractiveIndex) : refractiveIndex(RefractiveIndex) {}

	std::tuple<bool, Vec3F, Ray> scatter(Ray const& ray, HitRecord const& rec) override
	{
		auto reflected = reflect(ray.Direction(), rec.normal);
		Vec3F attenuation(1, 1, 1);
		Vec3F outwardNormal;
		double niOverNt;
		double cosine;
		
		auto normalizedRayProject = dot(ray.Direction(), rec.normal) / ray.Direction().Length();

		if (dot(ray.Direction(), rec.normal) > 0)
		{
			outwardNormal = -rec.normal;
			niOverNt = refractiveIndex;
			cosine = normalizedRayProject;
			cosine = std::sqrt(1 - refractiveIndex * refractiveIndex * (1 - cosine * cosine));
		}
		else
		{
			outwardNormal = rec.normal;
			niOverNt = 1.0 / refractiveIndex;
			cosine = -normalizedRayProject;
		}

		double reflectProbability;
		Ray scattered(Vec3F{}, Vec3F{});

		Vec3F refractedVec = reflected;
		if (auto[refracted, refractedV] = refract(ray.Direction(), outwardNormal, niOverNt); refracted)
		{
			reflectProbability = schlick(cosine, refractiveIndex);
			refractedVec = refractedV.value();
		}
		else 
		{
			reflectProbability = 1.0;
		}

		if (dist(ranDevice) < reflectProbability)
		{
			scattered = Ray(rec.p, reflected);
		}
		else
		{
			scattered = Ray(rec.p, refractedVec);
		}

		return std::make_tuple(true, Vec3F(1, 1, 1), scattered);
	}

private:
	double schlick(double cosine, double ri)
	{
		auto r0 = (1 - ri) / (1 + ri);
		r0 = r0 * r0;
		return r0 + (1 - r0)*std::pow((1 - cosine), 5);
	}

	std::tuple<bool, std::optional<Vec3F>> refract(Vec3F const& v, Vec3F const& n, double niOverNt)
	{
		auto uv = MakeUnitVector(v);
		auto dt = dot(uv, n);
		auto discriminant = 1.0 - niOverNt * niOverNt * (1.0 - dt * dt);
		if (discriminant > 0)
		{
			auto refracted = niOverNt * (uv - n * dt) - n * std::sqrt(discriminant);
			return std::make_tuple(true, refracted);
		}
		else
		{
			return std::make_tuple(false, std::nullopt);
		}
	}

	double refractiveIndex;
};