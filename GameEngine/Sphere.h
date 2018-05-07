#pragma once
#include "IRayHitable.h"
#include "Vec3.h"
#include <memory>

class Sphere : public IRayHitable
{
public:
	Sphere(Vec3F c, double r, std::unique_ptr<IMaterial>&& m) : center(c), radius(r), mat(std::move(m)) {}

	bool hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const override
	{
		rec.mat = mat.get();
		auto oc = r.Origin() - center;
		auto a = dot(r.Direction(), r.Direction());
		auto b = dot(oc, r.Direction());
		auto c = dot(oc, oc) - radius * radius;
		auto discriminant = b * b - a * c;
		if (discriminant < 0)
		{
			return false;
		}
		else {
			auto temp = (-b - std::sqrt(discriminant)) / a;
			if (temp < tMax && temp > tMin)
			{
				rec.t = temp;
				rec.p = r.PointAtParameter(rec.t);
				rec.normal = (rec.p - center) / radius;
				return true;
			}
			temp = (-b + std::sqrt(discriminant)) / a;
			if (temp < tMax && temp > tMin)
			{
				rec.t = temp;
				rec.p = r.PointAtParameter(rec.t);
				rec.normal = (rec.p - center) / radius;
				return true;
			}
			return false;
		}
	}
private:
	Vec3F center;
	double radius;
	std::unique_ptr<IMaterial> mat;
};