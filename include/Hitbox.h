#ifndef HITBOX_H
#define HITBOX_H

#include "Plane.h"

class Hitbox {
public:
	Plane planes[6]; // 有6个面
	Hitbox(glm::vec3 center, float x, float y, float z);
	// 算出被击中点到发射点的距离，如果没击中那么返回负数
	float isHit(vec3 viewPos, vec3 viewDir);
};

void divideByW(glm::vec4& mat);
#endif