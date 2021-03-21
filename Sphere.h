#pragma once

#include "Geometry.h"

/*
* Sphere Geometry class
*/
class Sphere : public Geometry {

public:
	// Constructor
	Sphere(Coordinate origin, GLfloat radius, GLuint sectorCnt, GLuint stackCnt) {
		
		// set origin
		origin_ = Coordinate(0.0, 0.0, 0.0);

		// set radius
		radius_ = radius;

		// clear memory space
		std::vector<GLfloat>().swap(vertices_);
		std::vector<GLfloat>().swap(normals_);
		std::vector<GLfloat>().swap(texture_coords_);

		GLfloat x, y, z, xy;    // vertex position
		GLfloat nx, ny, nz;    // vertex normal
		GLfloat lengthInv = 1.0f / radius;    // vertex normal norm
		GLfloat s, t;    // vertex texture coordinate

		GLfloat sectorStep = 2 * PI / sectorCnt;
		GLfloat stackStep = PI / stackCnt;
		GLfloat sectorAngle, stackAngle;

		for (int i = 0; i <= stackCnt; ++i) {
			stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
			xy = radius_ * cosf(stackAngle);             // r * cos(u)
			z = radius_ * sinf(stackAngle);              // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= sectorCnt; ++j)
			{
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position (x, y, z)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
				vertices_.push_back(x);
				vertices_.push_back(y);
				vertices_.push_back(z);

				// normalized vertex normal (nx, ny, nz)
				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;
				normals_.push_back(nx);
				normals_.push_back(ny);
				normals_.push_back(nz);

				// vertex tex coord (s, t) range between [0, 1]
				s = (float)j / sectorCnt;
				t = (float)i / stackCnt;
				texture_coords_.push_back(s);
				texture_coords_.push_back(t);
			}
		}

		/* generate CCW index list of sphere triangles
		k1--k1+1
		|  / |
		| /  |
 		k2--k2+1
		*/
		int k1, k2;
		for (int i = 0; i < stackCnt; ++i)
		{
			k1 = i * (sectorCnt + 1);     // beginning of current stack
			k2 = k1 + sectorCnt + 1;      // beginning of next stack

			for (int j = 0; j < sectorCnt; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					indices_.push_back(k1);
					indices_.push_back(k2);
					indices_.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (stackCnt - 1))
				{
					indices_.push_back(k1 + 1);
					indices_.push_back(k2);
					indices_.push_back(k2 + 1);
				}

				// store indices for lines
				// vertical lines for all stacks, k1 => k2
				lineIndices_.push_back(k1);
				lineIndices_.push_back(k2);
				if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
				{
					lineIndices_.push_back(k1);
					lineIndices_.push_back(k1 + 1);
				}
			}
		}

	}

	// DrawObj method
	// Initiate draw call to OpenGL using current shader state
	virtual void DrawObj(ShaderState& curSS) final {

	}

private:
	Coordinate origin_;
	GLfloat radius_;
	std::vector<GLfloat> vertices_;
	std::vector<GLfloat> normals_;
	std::vector<GLfloat> texture_coords_;
	std::vector<GLint> indices_;
	std::vector<GLint> lineIndices_;
};