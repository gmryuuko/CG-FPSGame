#include "Plane.h"

#include<iostream>
using namespace std;
void Plane::setupBoundary() {
	normal = glm::cross(xdir, ydir);
	upperx = center.x + (xdir * a).x / 2 + (ydir * b).x / 2 + MARGIN;
	uppery = center.y + (xdir * a).y / 2 + (ydir * b).y / 2 + MARGIN;
	upperz = center.z + (xdir * a).z / 2 + (ydir * b).z / 2 + MARGIN;
	lowerx = center.x - (xdir * a).x / 2 - (ydir * b).x / 2 - MARGIN;
	lowery = center.y - (xdir * a).y / 2 - (ydir * b).y / 2 - MARGIN;
	lowerz = center.z - (xdir * a).z / 2 - (ydir * b).z / 2 - MARGIN;
}


float Plane::isHit(const glm::vec3 viewPos, const glm::vec3 viewDir) {
	glm::vec3 rayToPlane = center - viewPos;
	float scaleFactor = glm::dot(rayToPlane, normal) / glm::dot(viewDir, normal);
	if (scaleFactor <= 0 || scaleFactor == NAN || scaleFactor == INFINITY) return INFINITY;
	glm::vec3 hitPos = viewPos + viewDir * scaleFactor;
	// 检测是否在长方形内部
	//cout << "\nThe hit position is:" << hitPos.x << ", " << hitPos.y << ", " << hitPos.z << endl;
	//cout << "The view position is:" << viewPos.x << ", " << viewPos.y << ", " << viewPos.z << endl;
	//cout << "The view direction is:" << viewDir.x << ", " << viewDir.y << ", " << viewDir.z << endl;
	if (hitPos.x > lowerx && hitPos.x < upperx && hitPos.y > lowery && hitPos.y < upperz && hitPos.z >lowerz && hitPos.z < upperz) {
		return glm::length(viewDir) * scaleFactor;
	}
	return INFINITY;
}
