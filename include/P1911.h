#pragma once
#include "Gun.h"
class P1911 : public Gun {
public:
	P1911() {
		recoil = 10;
		recenteringTime = 0.2;
		damage = 62;

		linear = 0.7;
		quadratic = 1.8;
		setConst();
	}
};