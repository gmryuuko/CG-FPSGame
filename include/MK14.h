#pragma once
#include "Gun.h"
class MK14: public Gun{
public:
	MK14() {
		recoil = 6;
		recenteringTime = 0.5;
		damage = 42;

		linear = 0.014;
		quadratic = 0.007;
		setConst();
	}
};