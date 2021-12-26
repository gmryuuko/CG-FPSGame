#pragma once
#include "Gun.h"
class LittleGun : public Gun {
public:
	LittleGun() {
		recoil = 0;
		recenteringTime = 0;
		damage = 15;

		linear = 1.7;
		quadratic = 2.8;
		setConst();
	}
};