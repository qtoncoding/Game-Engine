#pragma once
#include "IRayHitable.h"
#include <vector>
#include <memory>
#include <algorithm>

using IRayHitableVector = std::vector<std::unique_ptr<IRayHitable>>;

class RayHitableList : public IRayHitable
{
public:
	RayHitableList(IRayHitableVector&& v) : objects(std::move(v)) {}
	bool hit(Ray const& r, double tMin, double tMax, HitRecord& rec) const override 
	{
		HitRecord tempRec;
		bool hitAnything = false;
		double closest = tMax;
		for (auto& o : objects)
		{
			if (o->hit(r, tMin, closest, tempRec))
			{
				hitAnything = true;
				closest = tempRec.t;
				rec = tempRec;
			}
		}

		return hitAnything;
	}
private:
	std::vector<std::unique_ptr<IRayHitable>> objects;
};