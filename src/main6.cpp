/*
[notes]
6 - using surface normals to shade an object and implementing the algo for multiple objects
- created a sphere with flat coloring via checking whether or not we had an intersection and coloring it red
- this time instead of a binary decision of if we saw: make it red, our hit_sphere method maps an intensity to that hit point for the RGB color

***outside resources used***
Fundamentals of Computer Graphics (ch1,4) + adhoc googling
https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html
https://youtu.be/gBPNO6ruevk?si=gy161KOT-mdYd-F8
https://www.learncpp.com/cpp-tutorial/virtual-functions/
https://www.learncpp.com/cpp-tutorial/destructors/
https://www.learncpp.com/cpp-tutorial/basic-inheritance-in-c/
https://www.learncpp.com/cpp-tutorial/the-override-and-final-specifiers-and-covariant-return-types/
https://www.learncpp.com/cpp-tutorial/stdshared_ptr/

prog so far:
each pixel, check if our ray is hitting the sphere 
if it hits then map the components of that point to [0,1] 

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
	- create a ray traced image w/ a sphere using ray-sphere intersection calculations
	- shading a sphere w/ surface normals (vectors perpendicular to the sphere at a point of intersection)
		- design decisions for unit length (some direction, length 1) normal (perpendicular to a point) vectors:
			1 are the surface normals an arbritrary length or unit-length?
				- we do eventually require unit-length surface normals somewhere
				- calculating them is inexpensive in the case of a sphere, normalizing a vector 
					- how do we get a vector perpendicular to the surface of the sphere? 
						- simple: this is informal but just take the line of the radius and extended it out from the surface point
						- we can then just normalize via dividing by the length of the radius
			2 should normals always point outwards? 
				- motivation: which side of the surface are the viewing rays coming from
					- objects with insides that are different (ie glass balls, things that look different depending on side)
				- currently (pre 6.4) it points outwards, recall we use the radius and extended it out then normalize
				- recall: the outward normal is in the direction of (origin to the hit point)
				- case: always pointing outwards from surface (side determined at the time of coloring)
					- case: pointing same direction (ray and normal)
						- negative ray times negative normal 
						- positive ray times positive normal
					- case: pointing opposite directions (ray and normal)
						- positive ray/normal * negative ray/normal
						- negative ray/normal * positive ray/normal
					- tradeoffs: requires dot product calculation
				- use case:
					- determine side of surface at a specific time
						- geometry intersection
						- coloring
					- the book focuses on material types (stuff on the surface) so we'll focus on time of geometry intersection
						- pointing against ray?
		- ***from now on ALL of our surface normals will be unit length***
		- visualizing normals 
			- let n be a unit length vector
			- map its components (ranging from -1 to 1) to 0 to 1 (via adding 1 and then dividing by half)
			- now the 0 to 1 mapped components can be used for the intensity of each part of RGB
				- wish this was fucking explained earlier
	- simplifying ray-sphere intersection
		- i have no idea what the significance of this simplification means
		- substitute b for -2h in the quadratic equation
	- creating a class for "hittable" objects, ie objects that are hit by our viewing rays 
		
*/

#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

color ray_color(const ray& r, const hittable& world) {
	hit_record rec;
	if (world.hit(r, 0, infinity, rec)) {
		return 0.5 * (rec.normal + color(1, 1, 1));
	}

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
	
	// world, referred to as argument for ray_color function
	hittable_list world;

	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

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

			color pixel_color = ray_color(r, world);
			write_color(std::cout, pixel_color);
		}
	}
	std::clog << "\rDone.                 \n";
}
