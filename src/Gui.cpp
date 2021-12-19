#include "Gui.h"

#include <imgui-1.85/imgui.h>
#include <imgui-1.85/imgui_impl_glfw.h>
#include <imgui-1.85/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

namespace Gui {
	void InitGui(GLFWwindow* window) {
		// set dear imgui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// set color style
		ImGui::StyleColorsDark();

		// set platform/renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");
	}

	void NewFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Render() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Shutdown() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ShowObjectSettings(const Scene& scene) {
		ImGui::Begin("Objects");

		char objStr[1000] = { '\0' };
		static int chosen = 0;

		for (int i = 0, head = 0; i < scene.gameObjects.size(); i++) {
			std::string str = std::string("Object") + std::to_string(i);
			memcpy(objStr + head, str.c_str(), str.length());
			head += str.length() + 1;
		}
		ImGui::Combo("Choose a object", &chosen, objStr);


		static GameObject* chosenObj = nullptr;
		static glm::vec3 position, rotation, scale;

		if (chosenObj != scene.gameObjects[chosen]) {
			chosenObj = scene.gameObjects[chosen];
			position = chosenObj->transform->GetPosition();
			rotation = chosenObj->transform->GetRotation();
			scale = chosenObj->transform->GetScale();
		}

		if (chosenObj != nullptr) {
			ImGui::Text("Position");
			ImGui::DragFloat("position.x", &position.x, 0.05);
			ImGui::DragFloat("position.y", &position.y, 0.05);
			ImGui::DragFloat("position.z", &position.z, 0.05);
			ImGui::Text("Rotation");
			ImGui::DragFloat("rotation.x", &rotation.x, 0.2);
			ImGui::DragFloat("rotation.y", &rotation.y, 0.2);
			ImGui::DragFloat("rotation.z", &rotation.z, 0.2);
			ImGui::Text("Scale");
			ImGui::DragFloat("scale.xyz", &scale.x, 0.01, 0.01, 1000);
			if (position != chosenObj->transform->GetPosition())
				chosenObj->transform->SetPosition(position);
			if (rotation != chosenObj->transform->GetRotation())
				chosenObj->transform->SetRotation(rotation);
			if (glm::vec3(scale.x) != chosenObj->transform->GetScale())
				chosenObj->transform->SetScale(glm::vec3(scale.x));
		}

		ImGui::End();
	}


	void ShowDirLightSettings(const Scene& scene) {
		ImGui::Begin("Direction Lights");

		char objStr[1000] = { '\0' };
		static int chosen = 0;

		for (int i = 0, head = 0; i < scene.dirLights.size(); i++) {
			std::string str = std::string("Direction light ") + std::to_string(i);
			memcpy(objStr + head, str.c_str(), str.length());
			head += str.length() + 1;
		}

		if (scene.dirLights.empty()) {
			ImGui::Text("No direction light.");
		}
		else {
			ImGui::Combo("Choose a light", &chosen, objStr);

			static Light::DirLight *chosenLight = nullptr;

			if (chosenLight != scene.dirLights[chosen]) {
				chosenLight = scene.dirLights[chosen];
			}

			if (chosenLight != nullptr) {
				ImGui::Text("Direction");
				ImGui::DragFloat("direction.x", &chosenLight->direction.x, 0.1);
				ImGui::DragFloat("direction.y", &chosenLight->direction.y, 0.1);
				ImGui::DragFloat("direction.z", &chosenLight->direction.z, 0.1);
				ImGui::Text("Ambient");
				ImGui::DragFloat("ambient.r", &chosenLight->ambient.x, 0.005, 0, 1);
				ImGui::DragFloat("ambient.g", &chosenLight->ambient.y, 0.005, 0, 1);
				ImGui::DragFloat("ambient.b", &chosenLight->ambient.z, 0.005, 0, 1);
				ImGui::Text("Diffuse");
				ImGui::DragFloat("diffuse.r", &chosenLight->diffuse.x, 0.005, 0, 1);
				ImGui::DragFloat("diffuse.g", &chosenLight->diffuse.y, 0.005, 0, 1);
				ImGui::DragFloat("diffuse.b", &chosenLight->diffuse.z, 0.005, 0, 1);
				ImGui::Text("Specular");
				ImGui::DragFloat("specular.r", &chosenLight->specular.x, 0.005, 0, 1);
				ImGui::DragFloat("specular.g", &chosenLight->specular.y, 0.005, 0, 1);
				ImGui::DragFloat("specular.b", &chosenLight->specular.z, 0.005, 0, 1);
			}
		}

		ImGui::End();
	}

	void ShowPointLightSettings(const Scene& scene) {
		ImGui::Begin("Point Lights");

		char objStr[1000] = { '\0' };
		static int chosen = 0;

		for (int i = 0, head = 0; i < scene.pointLights.size(); i++) {
			std::string str = std::string("Point light ") + std::to_string(i);
			memcpy(objStr + head, str.c_str(), str.length());
			head += str.length() + 1;
		}

		if (scene.pointLights.empty()) {
			ImGui::Text("No point light.");
		}
		else {
			ImGui::Combo("Choose a light", &chosen, objStr);

			static Light::PointLight* chosenLight = nullptr;

			chosenLight = scene.pointLights[chosen];

			if (chosenLight != nullptr) {
				ImGui::Text("Position");
				ImGui::DragFloat("position.x", &chosenLight->position.x, 0.05);
				ImGui::DragFloat("position.y", &chosenLight->position.y, 0.05);
				ImGui::DragFloat("position.z", &chosenLight->position.z, 0.05);
				ImGui::Text("Ambient");
				ImGui::DragFloat("ambient.r", &chosenLight->ambient.x, 0.005, 0, 1);
				ImGui::DragFloat("ambient.g", &chosenLight->ambient.y, 0.005, 0, 1);
				ImGui::DragFloat("ambient.b", &chosenLight->ambient.z, 0.005, 0, 1);
				ImGui::Text("Diffuse");
				ImGui::DragFloat("diffuse.r", &chosenLight->diffuse.x, 0.005, 0, 1);
				ImGui::DragFloat("diffuse.g", &chosenLight->diffuse.y, 0.005, 0, 1);
				ImGui::DragFloat("diffuse.b", &chosenLight->diffuse.z, 0.005, 0, 1);
				ImGui::Text("Specular");
				ImGui::DragFloat("specular.r", &chosenLight->specular.x, 0.005, 0, 1);
				ImGui::DragFloat("specular.g", &chosenLight->specular.y, 0.005, 0, 1);
				ImGui::DragFloat("specular.b", &chosenLight->specular.z, 0.005, 0, 1);
				ImGui::Text("Attenuation");
				ImGui::DragFloat("linear", &chosenLight->linear);
				ImGui::DragFloat("quadratic", &chosenLight->quadratic);
			}
		}

		ImGui::End();
	}

	void ShowSpotLightSettings(const Scene& scene) {
		ImGui::Begin("Spot lights");

		char objStr[1000] = { '\0' };
		static int chosen = 0;

		for (int i = 0, head = 0; i < scene.spotLights.size(); i++) {
			std::string str = std::string("Spot light ") + std::to_string(i);
			memcpy(objStr + head, str.c_str(), str.length());
			head += str.length() + 1;
		}

		if (scene.spotLights.empty()) {
			ImGui::Text("No spot light.");
		}
		else {
			ImGui::Combo("Choose a light", &chosen, objStr);

			static Light::SpotLight* chosenLight = nullptr;

			chosenLight = scene.spotLights[chosen];

			if (chosenLight != nullptr) {
				ImGui::Text("Position");
				ImGui::DragFloat("position.x", &chosenLight->position.x, 0.05);
				ImGui::DragFloat("position.y", &chosenLight->position.y, 0.05);
				ImGui::DragFloat("position.z", &chosenLight->position.z, 0.05);
				ImGui::Text("Direction");
				ImGui::DragFloat("direction.x", &chosenLight->direction.x, 0.1);
				ImGui::DragFloat("direction.y", &chosenLight->direction.y, 0.1);
				ImGui::DragFloat("direction.z", &chosenLight->direction.z, 0.1);
				ImGui::Text("Ambient");
				ImGui::DragFloat("ambient.r", &chosenLight->ambient.x, 0.005, 0, 1);
				ImGui::DragFloat("ambient.g", &chosenLight->ambient.y, 0.005, 0, 1);
				ImGui::DragFloat("ambient.b", &chosenLight->ambient.z, 0.005, 0, 1);
				ImGui::Text("Diffuse");
				ImGui::DragFloat("diffuse.r", &chosenLight->diffuse.x, 0.005, 0, 1);
				ImGui::DragFloat("diffuse.g", &chosenLight->diffuse.y, 0.005, 0, 1);
				ImGui::DragFloat("diffuse.b", &chosenLight->diffuse.z, 0.005, 0, 1);
				ImGui::Text("Specular");
				ImGui::DragFloat("specular.r", &chosenLight->specular.x, 0.005, 0, 1);
				ImGui::DragFloat("specular.g", &chosenLight->specular.y, 0.005, 0, 1);
				ImGui::DragFloat("specular.b", &chosenLight->specular.z, 0.005, 0, 1);
				ImGui::Text("Attenuation");
				ImGui::DragFloat("linear", &chosenLight->linear);
				ImGui::DragFloat("quadratic", &chosenLight->quadratic);
			}
		}

		ImGui::End();
	}

	void ShowLightSettings(const Scene& scene) {
		ShowDirLightSettings(scene);
		ShowPointLightSettings(scene);
		ShowSpotLightSettings(scene);
	}


	void ShowSettings(const Scene& scene) {

		ImGui::Begin("Settings");

		static bool showObjects = true;
		static bool showLights = false;
		ImGui::Checkbox("Objects", &showObjects);
		ImGui::Checkbox("Lights", &showLights);

		ImGui::End();

		// other windows
		if (showObjects) ShowObjectSettings(scene);
		if (showLights) ShowLightSettings(scene);
	}
}