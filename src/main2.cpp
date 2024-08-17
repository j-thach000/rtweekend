#include <iostream>
/*
[notes]
2 - output a ppm image and add a progress indicator 
***outside resources used***
Fundamentals of Computer Graphics (ch1,3,4) + adhoc googling
- no idea wtf were raster images
- no background context on computer graphics in general
- missing lots of small context/background on stuff like color/rgb/overarching theory

key points
- recall that the point of the ray tracer is to render (color) 3d scenes
- ray-tracing itself is an image-order algorithm (turn 3d scenes into arrays of pixels)
- the ray tracer is this program itself that implements the algorithm

pedagogy:
- if we want to render scenes then obv we need to be able to output images first

image:
- 256 x 256 image
- each row is a certain amount of green with increasing intensity of red

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
			// let i and j represent the current pixel we're on (like a 2d coordinate plane)
			auto r = double(i) / (image_width-1);  // current intensity of red
			auto g = double(j) / (image_height-1); // current intensity of green
			auto b = 0.0;						   // no blue in image
			
			// convert the values to between [0,255]
			int ir = int(255.999 * r);
			int ig = int(255.999 * g);
			int ib = int(255.299 * b);

			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}
	std::clog << "\rDone.                 \n";
}
