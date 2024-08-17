/*
[notes]
so progression so far:
1) background info
	- history of computer graphics 
	- cs theory (designing classes, bits, bytes, use of float vs double)
	- raster images
	- color
	- overview of the ray tracing algorithm (image-order)
2) output a raster image
3) design a class to represent vectors and colors
4) design a class to represent the rays being shot out from each pixel

4 - ray (line shot out from pixel to look in a direction) class creation
***outside resources used***
Fundamentals of Computer Graphics (ch4) + adhoc googling
https://www.learncpp.com/cpp-tutorial/header-files/
https://www.learncpp.com/cpp-tutorial/constructor-member-initializer-lists/

function of a ray:
P(t) = A + tb
let P be a position in 3d space along some line
A is the origin of the ray (where the pixel is looking from)
b is the ray direction (some vector if it's a direction right)
t is some real number (double in the code), that modifies how far along the line we are in that direction

so tb is scalar multiplication

const keyword use:
1) before: affects the return type
2) after the name of the function and its arguments: this member function can't modify member variables of this class
*/



#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
	private:
		point3 orig;
		vec3 dir;

	public: 
		ray() {}
		// constructor member initializer list
		ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}
		
		const point3& origin() const { return orig; }
		const vec3& direction() const { return dir; }
	
		point3 at(double t) const {
			return orig + (t * dir);
		}
};
#endif
