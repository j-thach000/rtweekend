#include "vec3.h"
#include "color.h"

#include <iostream>
/*
[notes]
3 - reimplement the code to produce a raster image w/ the vector and color class
***outside resources used***
Fundamentals of Computer Graphics (ch1) + adhoc googling
*/
int main()
{
	// dimensions of our image
	int image_width = 256;
	int image_height = 256;

	// render the image
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	
	// set the color of pixels of a 256x256 image that contains no blue and gradients of red/green
	// from left to right, top to bottom: print out the rgb value of that current pixel for the image
	for (int j = 0; j < image_height; j++) {
		// logging w/ progress indicator, using clog doesn't cause these messages to print in the ppm file
		std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
		for (int i = 0; i < image_width; i++) {
			// what intensity is each component of rgb at?
			auto pixel_color = color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0);
			write_color(std::cout, pixel_color);
		}
	}
	std::clog << "\rDone.                 \n";
}
