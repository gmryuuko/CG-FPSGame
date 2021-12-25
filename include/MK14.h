#pragma once
#include "Gun.h"
class MK14: public Gun{
public:
	MK14() {
		recoil = 10;
		recenteringTime = 24;
		damage = 42;
	}
};