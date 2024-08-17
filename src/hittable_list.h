#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "rtweekend.h"

#include <vector>


class hittable_list : public hittable {
public:
	std::vector<shared_ptr<hittable>> objects;
	
	hittable_list() {}
	hittable_list(shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<hittable> object) {
		objects.push_back(object);
	}

    // process all the hittable objects in a list, modifying that object's hit record
    // constant keyword: a value that can't change during run-time
        // can't use a non-const function w/ a const argument
        // A const member function is a member function that guarantees it will not modify
        // the object or call any non-const member functions (as they may modify the object) of that instance
    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_tmax;

        for (const auto& object : objects) {
            if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};
#endif