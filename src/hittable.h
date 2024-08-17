#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

// contains all information at the point of intersection
class hit_record {
public:
	point3 p;
	vec3 normal;
	double t;
	bool front_face;

	void set_face_normal(const ray& r, const vec3& outward_normal) {
		// Sets the hit record normal vector.
		// NOTE: the parameter `outward_normal` is assumed to have unit length.

		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	// virtual functions make sure we refer to the derived class (ex: sphere and not hittable)
	virtual ~hittable() = default; // ~ is a destructor, destroy the hittable object
	virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
	
};

#endif