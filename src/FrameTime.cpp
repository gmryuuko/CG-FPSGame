#include "..\include\FrameTime.h"

#include "Time.h"

namespace FrameTime {
float lastFrameTime = 0;
float currentFrameTime = 0;
float deltaTime = 0;

void UpdateTime() {
    lastFrameTime = currentFrameTime;
    currentFrameTime = (float)glfwGetTime();
    deltaTime = currentFrameTime - lastFrameTime;
}

float GetTime() { return currentFrameTime; }

float GetDeltaTime() { return deltaTime; }
}  // namespace FrameTime