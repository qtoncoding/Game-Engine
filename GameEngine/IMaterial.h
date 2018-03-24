#pragma once
#include "Ray.h"
#include "Vec3.h"
#include "IRayHitable.h"
#include <tuple>
class IMaterial
{
public:
	virtual std::tuple<bool, Vec3F, Ray> scatter(Ray const& r, HitRecord const& rec) = 0;
};