#include "Hitbox.h"
#include<iostream>
Hitbox::Hitbox(glm::vec3 center, float x, float y, float z) {
	this->center = center;
	this->x = x; this->y = y; this->z = z;
	upperx = center.x + x / 2 + padding;
	uppery = center.y + y / 2 + padding;
	upperz = center.z + z / 2 + padding;
	lowerx = center.x - x / 2 - padding;
	lowery = center.y - y / 2 - padding;
	lowerz = center.z - z / 2 - padding;

	planes[0].xdir = glm::vec3(0, 0, 1);
	planes[0].ydir = glm::vec3(0, 1, 0);
	planes[0].a = z, planes[0].b = y;
	planes[0].center = glm::vec3(center.x + x / 2, center.y, center.z);

	planes[1].xdir = glm::vec3(0, 0, 1);
	planes[1].ydir = glm::vec3(0, 1, 0);
	planes[1].a = z, planes[1].b = y;
	planes[1].center = glm::vec3(center.x - x / 2, center.y, center.z);

	planes[2].xdir = glm::vec3(1, 0, 0);
	planes[2].ydir = glm::vec3(0, 1, 0);
	planes[2].a = x, planes[2].b = y;
	planes[2].center = glm::vec3(center.x, center.y, center.z + z / 2);

	planes[3].xdir = glm::vec3(1, 0, 0);
	planes[3].ydir = glm::vec3(0, 1, 0);
	planes[3].a = x, planes[3].b = y;
	planes[3].center = glm::vec3(center.x, center.y, center.z - z / 2);

	planes[4].xdir = glm::vec3(1, 0, 0);
	planes[4].ydir = glm::vec3(0, 0, 1);
	planes[4].a = x, planes[4].b = z;
	planes[4].center = glm::vec3(center.x, center.y + y / 2, center.z);

	planes[5].xdir = glm::vec3(1, 0, 0);
	planes[5].ydir = glm::vec3(0, 0, 1);
	planes[5].a = x, planes[5].b = z;
	planes[5].center = glm::vec3(center.x, center.y - y / 2, center.z);
	for (int i = 0; i < 6; i++)
		planes[i].setupBoundary();
}
// 算出被击中点到发射点的距离，如果没击中那么返回负数
float Hitbox::isHit(const vec3& viewPos, const vec3& viewDir) {
	float dist = INFINITY;
	for (int i = 0; i < 6; i++) {
		float temp = planes[i].isHit(viewPos, viewDir);
		if (temp < dist && temp > 0)
			dist = temp;
	}
	return dist;
}
bool Hitbox::isInside(vec4& modelPos) {	
	float x1, x2, y1, y2, z1, z2;
	x1 = modelPos.x - lowerx;
	x2 = upperx - modelPos.x;
	y1 = modelPos.y - lowery;
	y2 = uppery - modelPos.y;
	z1 = modelPos.z - lowerz;
	z2 = upperz - modelPos.z;
	if (x1 > 0 && x2 > 0 && y1 > 0 && y2 > 0 && z1 > 0 && z2 > 0) {
		
		// 算出它离哪个平面最近
		
		//std::cout << "The modelPos is:" << modelPos.x << ", " << modelPos.y << ", " << modelPos.z << std::endl;
		float min = INFINITY;
		if (min > x1) min = x1;
		if (min > x2) min = x2;
		if (min > y1) min = y1;
		if (min > y2) min = y2;
		if (min > z1) min = z1;
		if (min > z2) min = z2;

		// 移动到相应的平面上去
		if (min == x1) {
			modelPos.x = lowerx - MARGIN;
		}
		else if (min == x2) {
			modelPos.x = upperx + MARGIN;
		}
		else if (min == y1) {
			modelPos.y = lowery - MARGIN;
		}
		else if (min == y2) {
			modelPos.y = uppery + MARGIN;
		}
		else if (min == z1) {
			modelPos.z = lowerz - MARGIN;
		}
		else if (min == z2) {
			modelPos.z = upperz + MARGIN;
		}
		//std::cout << "The modelPos is:" << modelPos.x << ", " << modelPos.y << ", " << modelPos.z << std::endl << std::endl;
		
		
		return true;
	}
		
	return false;
}

void divideByW(glm::vec4& mat) {
	if (mat.w != 0) {
		mat.x /= mat.w;
		mat.y /= mat.w;
		mat.z /= mat.w;
		mat.w = 1;
	}
}