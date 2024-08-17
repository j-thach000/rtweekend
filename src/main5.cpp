/*
[notes]
5 - use ray-sphere intersection calculations to render a sphere w/ simple boolean checks
***outside resources used***
Fundamentals of Computer Graphics (ch1,4) + adhoc googling
https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html
https://youtu.be/gBPNO6ruevk?si=gy161KOT-mdYd-F8

pedagogy:
	- background context
	- create a raster image
	- created classes to represent vectors, colors, and view rays
	- viewport 
		- vectors representing the bottom and top edges
		- point representing the top left edge 
		- pixel gaps represented as vectors
		- first pixel location
	- camera
		- focal length (distance of camera/eyepoint/viewport from the image)
		- origin
	- create a ray traced image w/ a sphere
		- ray-sphere intersection calculations
		- mathematically simple
			- let p be a point on a sphere, all of its coordinates squared (imagine x, y, z being vectors w/ the relevant component being x/y/z and the rest 0 
			  is equal to its radius squared (add all the components
			- so if LHS > RHS then the point is outside the circle and vice versa
		- if we want the sphere at an arbritrary point and to find all rays are hitting it, we must solve for all t that satisfy P(t) = A + tb
			- implies we're shifting it from the center
			- let P = (x,y,z)
			- let C = (0,0,0)
			- so the distance from C to P = C - P
			- manipulating that via dot product, we get 
				- (C - P) * (C - P) = r^2
					- motivation: we want our formulas to almost always be in terms of vectors
								  the signifance is that now any point satisfying this equation means it must be on the sphere
								  allows us to easily check if our ray in the form of P(t) = A + tb is hitting the sphere 
									- recall A is the origin
									- b is some vector (direction ray) scaled by t
				- basically if we expand this out and substitute P w/ P(t) and solve for t using the quadratic formula
					- the quadratic formula can result in:
						- no solution (ray isn't hitting the sphere at all)
						- 1 solution (ray is hitting the sphere tangent at one point)
						- 2 solutions (ray is hitting 1 point and going through the back to hit another point)
		    
				

*/
#include "vec3.h"
#include "ray.h"
#include "color.h"

#include <iostream>

bool hit_sphere(const point3& center, double radius, const ray& r) {
	vec3 oc = center - r.origin(); // C - P
	// rest is just implementing the quadratic equation after manipulating the sphere equation in terms of vectors to solve for t
	auto a = dot(r.direction(), r.direction());
	auto b = -2.0 * dot(r.direction(), oc);
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = b * b - 4 * a * c;
	return (discriminant >= 0);
}

color ray_color(const ray& r) {
	// if we hit points on this sphere that's at the origin, pushed back in (z = -1), w/ radius 1/2 then color the pixel red
	if (hit_sphere(point3(0, 0, -1), 0.5, r)) // -z implies it's in front of our camera
		return color(1, 0, 0);

	vec3 unit_direction = unit_vector(r.direction());
	auto a = 0.5 * (unit_direction.y() + 1.0); // let a represent the intensity of blue
											   // from our origin, y could be negative after traversing the viewport so add +1 to avoid having it negative
											   // recall the range of y of the viewport could have been from -1 to 1, which is total distance 2 (viewport height)
											   // divide the result by 1/2 to get the range to [0,1] to finish computing the blending factor
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7,1.0); // wtf???
}

int main()
{
	// image, make sure the height is at least 1 (otherwise what are we rendering)
	auto aspect_ratio = 16.0 / 9.0;
	int image_width = 400;
	int image_height = int(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	// camera and viewport settings
	auto focal_length = 1.0;
	auto camera_center = point3(0, 0, 0);
	
	auto viewport_height = 2.0; // viewport dimensions in arbritrary units
	auto viewport_width = viewport_height * (double(image_width) / image_height); // referencing original image height and width for the exact match
	auto viewport_u = vec3(viewport_width, 0, 0);
	auto viewport_v = vec3(0, -viewport_height, 0);
	auto pixel_delta_u = viewport_u / image_width; // spacing between pixels on the x-axis and below for the y-axis
	auto pixel_delta_v = viewport_v / image_height;
	
	auto viewport_upper_left = camera_center 
						     - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2; 
	auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	// render
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	
	// from left to right, top to bottom: print out the rgb value of that current pixel for the image
	for (int j = 0; j < image_height; j++) {
		// logging w/ progress indicator, using clog doesn't cause these messages to print in the ppm file
		std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
		for (int i = 0; i < image_width; i++) {
			auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v); // literal position of each pixel on each row, using starting pixel as base for calc
			auto ray_direction = pixel_center - camera_center; // imagine a line shooting out from a dot centered on the viewport, 1 unit away from the image
			ray r(camera_center, ray_direction); // origin, direction 

			color pixel_color = ray_color(r);
			write_color(std::cout, pixel_color);
		}
	}
	std::clog << "\rDone.                 \n";
}
