#include "Time.h"
#include "..\include\FrameTime.h"

float FrameTime::lastFrameTime = 0;
float FrameTime::currentFrameTime = 0;
float FrameTime::deltaTime = 0;

void FrameTime::UpdateTime() {
	lastFrameTime = currentFrameTime;
	currentFrameTime = (float)glfwGetTime();
	deltaTime = currentFrameTime - lastFrameTime;
}

float FrameTime::GetTime() {
	return currentFrameTime;
}

float FrameTime::GetDeltaTime() {
	return deltaTime;
}