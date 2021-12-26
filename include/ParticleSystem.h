#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ctime>
#include "stb_image.h"
#include "Shader.h"

#define LAUNCHER 0.0f
#define BULLET 1.0f

#define MAX_VELOCITY glm::vec3(0.0,5.0,0.0)
#define MIN_VELOCITY glm::vec3(0.0,2.0,0.0)
#define MAX_AGE 2.0f*1000
#define MIN_AGE 1.0f*1000  
#define PARTICLE_SIZE 10.0f;

const int MAX_PARTICLES = 10000;

struct particle
{
	float type;
	glm::vec3 position;
	glm::vec3 velocity;
	float age;//年龄
	float alpha;//alpha通道
	float size;//大小
	float lifeSpan;//寿命
};

class ParticleSystem
{
public:
	ParticleSystem(int num);
	void render(float frametimeMills, glm::mat4& worldMatrix, glm::mat4 viewMatrix, glm::mat4& projectMatrix);
private:
	GLuint particleBuffers[2];
	GLuint particleArrays[2];
	GLuint transformFeedbacks[2];
	Shader* updateShader;
	Shader* renderShader;
	unsigned int texture1, texture2;
	unsigned int currVB, currTFB;
	float timer;
	bool flag;
	int particleNum;
	bool initParticleSystem();
	void initLocation(particle partciles[], int nums);
	void updateParticles(float frametimeMills);
	void renderParticles(glm::mat4& worldMatrix, glm::mat4& viewMatrix, glm::mat4& projectMatrix);
};

//initLocation
//gs,fs
//texture

