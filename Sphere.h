#pragma once

#include "Geometry.h"

/*
* Sphere Geometry class
*/
class Sphere : public Geometry {

public:
	// Constructor
	// Creates an instance of sphere class
	Sphere(Coordinate origin, GLfloat radius, GLuint sectorCnt, GLuint stackCnt) {
		
		// set origin
		origin_ = Coordinate(0.0, 0.0, 0.0);

		// set radius
		radius_ = radius;

		// set resolution
		sectorCnt_ = sectorCnt;
		stackCnt_ = stackCnt;

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

		for (int i = 0; i <= stackCnt_; ++i) {
			stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
			xy = radius_ * cosf(stackAngle);             // r * cos(u)
			z = radius_ * sinf(stackAngle);              // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= sectorCnt_; ++j)
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
		for (int i = 0; i < stackCnt_; ++i)
		{
			k1 = i * (sectorCnt_ + 1);     // beginning of current stack
			k2 = k1 + sectorCnt_ + 1;      // beginning of next stack

			for (int j = 0; j < sectorCnt_; ++j, ++k1, ++k2)
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
				if (i != (stackCnt_ - 1))
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

		// generate interleaved expression of the instance
		buildInterleavedVertices();
	}

	// DrawObj method
	// Initiate draw call to OpenGL using current shader state
	virtual void DrawObj(ShaderState& curSS) final {

	// setters
	void setRadius(GLfloat radius) {
		radius_ = radius;
	}

	void setSectorCnt(GLuint sectorCnt) {
		sectorCnt_ = sectorCnt;
	}

	void setStackCnt(GLuint stackCnt) {
		stackCnt_ = stackCnt;
	}

	// getters
	void* getInterleavedVertices() {
		return &interleaved_vertices_[0];
	}

	size_t getInterleavedVertices_Size() {
		return interleaved_vertices_.size() * sizeof(interleaved_vertices_[0]);
	}

	void* getIndices() {
		return &indices_[0];
	}

	size_t getIndices_Size() {
		return indices_.size() * sizeof(indices_[0]);
	}

	void buildInterleavedVertices() {
		std::vector<GLfloat>().swap(interleaved_vertices_);

		std::size_t i, j;
		std::size_t count = vertices_.size();
		for (i = 0, j = 0; i < count; i += 3, j += 2)
		{
			interleaved_vertices_.push_back(vertices_[i]);
			interleaved_vertices_.push_back(vertices_[i + 1]);
			interleaved_vertices_.push_back(vertices_[i + 2]);

			interleaved_vertices_.push_back(normals_[i]);
			interleaved_vertices_.push_back(normals_[i + 1]);
			interleaved_vertices_.push_back(normals_[i + 2]);

			interleaved_vertices_.push_back(texture_coords_[j]);
			interleaved_vertices_.push_back(texture_coords_[j + 1]);
		}
	}

private:
	// gpu
	// Transfer geometry data to GPU
	void gpu() {

		// copy vertex data to VBO
		glGenBuffers(1, (GLuint*)&posVBO);
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		glBufferData(GL_ARRAY_BUFFER,
			getInterleavedVertices_Size(),
			getInterleavedVertices(),
			GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&idxVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			getIndices_Size(),
			getIndices(),
			GL_STATIC_DRAW);

		// copy attributes to VBO
		// TODO: Implement this!
	}

private:
	GlBufferObject posVBO, idxVBO, texVBO, colVBO;

	Coordinate origin_;
	GLfloat radius_;
	GLuint sectorCnt_;
	GLuint stackCnt_;
	std::vector<GLfloat> vertices_;
	std::vector<GLfloat> normals_;
	std::vector<GLfloat> texture_coords_;
	std::vector<GLint> indices_;
	std::vector<GLint> lineIndices_;
	std::vector<GLfloat> interleaved_vertices_;
};