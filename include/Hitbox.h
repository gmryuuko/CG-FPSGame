#ifndef HITBOX_H
#define HITBOX_H

#include "Plane.h"

const float padding = 0.5;

class Hitbox {
public:
	glm::vec3 center;
	float x, y, z;
	float upperx, uppery, upperz, lowerx, lowery, lowerz;

	Plane planes[6]; // 有6个面
	Hitbox(glm::vec3 center, float x, float y, float z);
	// 算出被击中点到发射点的距离，如果没击中那么返回负数
	float isHit(const vec3& viewPos, const vec3& viewDir);
	bool isInside(vec4& modelPos);

};

void divideByW(glm::vec4& mat);
#endif