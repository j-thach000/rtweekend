#ifndef COLOR_H
#define COLOR_H
/*
[notes]
3 - create a class to represent colors for pixels
***outside resources used***
Fundamentals of Computer Graphics (ch1) + adhoc googling

*/

#include "rtweekend.h"
#include "vec3.h"

using color = vec3; // color is stored as a vector of 3 components

// utility function that writes a single pixel's color to the output stream
// input: text to print and a reference to a pixel color which shouldn't be modified
void write_color(std::ostream& out, const color& pixel_color) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	// translate the [0,1] component values to [0,255], 255.999 to avoid truncation error
	int rbyte = int(255.999 * r);
	int gbyte = int(255.999 * g);
	int bbyte = int(255.999 * b);

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
#endif