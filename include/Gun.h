#pragma once
class Gun{
public:
	float recoil;
	float recenteringTime; //µ¥Î»Îªframetime
	int damage;
	Gun() {
		recoil = recenteringTime = 0;
		damage = 0;
	}
};