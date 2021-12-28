#pragma once
#include "FrameTime.h"
class Gun{
public:
	float recoil;
	float recenteringTime; //单位为frametime
	int damage;
	float lastHitTime;
	float constFactor;

	// 衰减系数
	float constant, linear, quadratic;

	Gun() {
		recoil = recenteringTime = 0;
		damage = 0;
		lastHitTime = 0;
		constFactor = 0;

		constant = 1; linear = quadratic = 0;
	}

	void setConst() {
		constFactor = 4 * recoil / recenteringTime / recenteringTime;
	}
	float gunRecoil() {
		
		float delta = FrameTime::GetTime() - lastHitTime;
		if (delta < recenteringTime) {
			return delta * (recenteringTime - delta) * constFactor;
		}
		else return 0;
	}
	void fires() {
		lastHitTime = FrameTime::GetTime();
	}
	float getDamage(float dist) {
		return damage / (constant + linear * dist + quadratic * dist * dist);
	}
};