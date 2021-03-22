#pragma once

#include <math.h>
#include <vector>
#include <tuple>

#include "glm/glm.hpp"
#include "ShaderState.h"
#include "Constants.h"

using Coordinate = glm::vec3;

/**
* Base class for geometries in the project.
* All geometry classes must be derived from this.
*/
class Geometry {
public:

	// Default Constructor
	Geometry() {
		// Do nothing
	}

	// DrawObj method
	// Initiate draw call to OpenGL using current shader state
	virtual void DrawObj(ShaderState& curSS) {
		// Do nothing
	}

private:
};
