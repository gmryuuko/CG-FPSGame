#pragma once

#ifndef GUI_H


#include "Scene.h"
#include <GLFW/glfw3.h>

namespace Gui {
	void InitGui(GLFWwindow* window);
	void NewFrame();
	void Shutdown();
	void ShowSettings(const Scene& scene);
	void Render();
}

#endif // !GUI_H
