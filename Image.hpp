// Termm--Fall 2020

#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <string.h>

#include "Ray.hpp"

typedef unsigned int uint;

/**
 * An image, consisting of a rectangle of floating-point elements.
 * Each pixel element consists of 3 components: Red, Blue, and Green.
 *
 * This class makes it easy to save the image as a PNG file.
 * Note that colours in the range [0.0, 1.0] are mapped to the integer
 * range [0, 255] when writing PNG files.
 */
class Image {
public:
	// Construct an empty image.
	Image();

	// Construct a black image at the given width/height.
	Image(uint width, uint height);

	// Copy an image.
	Image(const Image & other);

	~Image();

	// Copy the data from one image to another.
	Image & operator=(const Image & other);

	// Returns the width of the image.
	uint width() const;

	// Returns the height of the image.
	uint height() const;

    // Retrieve a particular component from the image.
	double operator()(uint x, uint y, uint i) const;

	// Retrieve a particular component from the image.
	double & operator()(uint x, uint y, uint i);

	// Save this image into the PNG file with name 'filename'.
	// Warning: If 'filename' already exists, it will be overwritten.
	bool savePng(const std::string & filename) const;

	bool loadPng(const std::string & filename);

	glm::vec3 operator()(uint x, uint y) const;

	bool empty() const;

	glm::vec3 get_color(double u, double v) const;

	const double * data() const;
	double * data();

protected:
	void setWidth(uint width) { this->m_width = width; }

	void setHeight(uint height) { this->m_height = height; }

	void setData(std::vector<unsigned char> image, size_t numElements) {
		this->m_data = new double[numElements];
		memset(m_data, 0, numElements*sizeof(double));
  		for (int i = 0; i < numElements; i++) {
    		m_data[i] = (double)image[i] / 255.0;
  		}
	};

private:
	uint m_width;
	uint m_height;
	double * m_data;

	static const uint m_colorComponents;
};
