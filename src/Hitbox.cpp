#include "Hitbox.h"
#include<iostream>
Hitbox::Hitbox(glm::vec3 center, float x, float y, float z) {
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
float Hitbox::isHit(vec3 viewPos, vec3 viewDir) {
	float dist = INFINITY;
	for (int i = 0; i < 6; i++) {
		float temp = planes[i].isHit(viewPos, viewDir);
		if (temp < dist && temp > 0)
			dist = temp;
	}
	return dist;
}

void divideByW(glm::vec4& mat) {
	if (mat.w != 0) {
		mat.x /= mat.w;
		mat.y /= mat.w;
		mat.z /= mat.w;
		mat.w = 1;
	}
}