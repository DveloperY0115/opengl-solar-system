#pragma once

#include "Geometry.h"

/*
* Sphere Geometry class
*/
class Sphere : public Geometry {

public:
	// Constructor
	Sphere() {
		// clear memory space
		std::vector<GLfloat>().swap(vertices);
		std::vector<GLfloat>().swap(normals);
		std::vector<GLfloat>().swap(texture_coords);


	}
private:
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texture_coords;
};