#pragma once
#include "Ray.h"
#include "Vec3.h"

class IMaterial;

struct HitRecord
{
	float t;
	Vec3F p;
	Vec3F normal;
	IMaterial* mat;
};

class IRayHitable 
{
public:
	virtual bool hit(Ray const& r, float tMin, float tMax, HitRecord& rec) const = 0;
};

