/*
[notes]
4 - send rays through pixels, find an intersection point (first point where the ray hits an object), and then color the pixel based on what the ray hit
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

camera implementation
	- for simplicity we start at (0,0,0)
	- focal length (dist between viewport (rectangle containing pixels) and camera) set to 1 unit
		- WHY????
			- it's to fit the image inside the viewport basically
	- camera geometry
		- y-axis is a vector that goes up 
			- means the image coordinate y-axis needs to be inverted
			- we go down by increasing Y
		- x-axis is a vector that goes right
		- negative z-axis points towards the viewport, from the camera
	- to actually navigate the pixel grid, we'll implement those x,y,z axis as vectors (points in 3d space that mark our viewport)
		- viewport vector Vu (x-axis) going right
		- viewport vector Vv (y-axis) going down
	- space between pixels
		- delta u
		- delta v

lerp between 2 values
	blendedValue = (1 - a) * startValue + a * endValue
	a goes from 0 to 1


*/
#include "vec3.h"
#include "ray.h"
#include "color.h"

#include <iostream>

color ray_color(const ray& r) {
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
