#pragma once

#ifndef TIME_H
#define TIME_H

#include <GLFW/glfw3.h>

class FrameTime {
private:
	static float lastFrameTime;
	static float currentFrameTime;
	static float deltaTime;

public:
	static void UpdateTime();
	static float GetTime();
	static float GetDeltaTime();
};

#endif // !TIME_H
