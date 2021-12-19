#ifndef  PLANE_H
#define PLANE_H

#include "glm/glm.hpp"
#include "glm/matrix.hpp"

#include <vector>

using namespace glm;

#define MARGIN 0.01

class Plane {
public:
	glm::vec3 center;
	glm::vec3 xdir, ydir, normal;
	float a, b;

	float upperx, uppery, upperz, lowerx, lowery, lowerz;

	void setupBoundary();

	float Plane::isHit(glm::vec3 viewPos, glm::vec3 viewDir);
};
#endif // ! PLANE_H