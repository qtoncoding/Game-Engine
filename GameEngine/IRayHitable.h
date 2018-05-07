#pragma once
#include "Ray.h"
#include "Vec3.h"

class IMaterial;

struct HitRecord
{
	double t;
	Vec3F p;
	Vec3F normal;
	IMaterial* mat;
};

class IRayHitable 
{
public:
	virtual bool hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const = 0;
};

