#pragma once

#ifndef TIME_H
#define TIME_H

#include <GLFW/glfw3.h>

namespace FrameTime {
	void UpdateTime();
	float GetTime();
	float GetDeltaTime();
};

#endif // !TIME_H
