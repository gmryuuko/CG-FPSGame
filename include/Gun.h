#pragma once
class Gun{
public:
	float recoil;
	float recenteringTime; //��λΪframetime
	int damage;
	Gun() {
		recoil = recenteringTime = 0;
		damage = 0;
	}
};