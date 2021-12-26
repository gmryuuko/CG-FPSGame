#include "ParticleSystem.h"

void loadTexture(const char* fileName, unsigned int* texture);

ParticleSystem::ParticleSystem(int num)
{
	std::cout << "???" << std::endl;
	glGetError();
	particleNum = num;
	currVB = 0;
	currTFB = 1;
	flag = true;
	timer = 0.0;
	const char* variables[7];
	variables[0] = "type_fs";
	variables[1] = "position_fs";
	variables[2] = "velocity_fs";
	variables[3] = "age_fs";
	variables[4] = "alpha_fs";
	variables[5] = "size_fs";
	variables[6] = "lifeSpan_fs";

	updateShader = new Shader("../shader/Update.vs", "../shader/Update.fs",
		"../shader/Update.gs", variables, 7);
	//设置TransformFeedback缓存能够记录的顶点的数据类型
	renderShader = new Shader("../shader/Render.vs", "../shader/Render.fs");

	loadTexture("../resources/textures/particle.bmp", &texture1);
	loadTexture("../resources/textures/flame.bmp", &texture2);

	renderShader->Use();
	renderShader->SetInt("flameSpark", 0);
	renderShader->SetInt("flameStart", 1);
	
	if (!initParticleSystem()) {
		std::cout << "initiate ParticleSystem Fail!" << std::endl;
	}

}

bool ParticleSystem::initParticleSystem()
{

	particle particles[MAX_PARTICLES];
	glm::vec3 initPosition(0.f, 0.f, 0.f);
	initLocation(particles, particleNum);

	glGenTransformFeedbacks(2, transformFeedbacks);
	glGenBuffers(2, particleBuffers);
	glGenVertexArrays(2, particleArrays);
	for (int i = 0; i < 2; i++)
	{
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedbacks[i]);
		glBindBuffer(GL_ARRAY_BUFFER, particleBuffers[i]);
		glBindVertexArray(particleArrays[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particleBuffers[i]);
	}

	return true;
}

void ParticleSystem::render(float dt, glm::mat4& worldMatrix,
	glm::mat4 viewMatrix, glm::mat4& projectMatrix)
{
	unsigned int tmp;
	timer += dt * 1000.0f;
	updateParticles(dt*1000.0f);
	renderParticles(worldMatrix, viewMatrix, projectMatrix);
	tmp = currVB;
	currVB = currTFB;
	currTFB = tmp;
}

void ParticleSystem::updateParticles(float dt)
{
	updateShader->Use();
	updateShader->SetFloat("dt", dt);
	updateShader->SetFloat("timer", timer);
	updateShader->SetFloat("MAX_AGE", MAX_AGE);
	updateShader->SetFloat("MIN_AGE", MIN_AGE);
	updateShader->SetVec3("MAX_VELOCITY", MAX_VELOCITY);
	updateShader->SetVec3("MIN_VELOCITY", MIN_VELOCITY);

	glEnable(GL_RASTERIZER_DISCARD);//gs更新粒子，不需要光栅
	glBindVertexArray(particleArrays[currVB]);
	glBindBuffer(GL_ARRAY_BUFFER, particleBuffers[currVB]);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedbacks[currTFB]);

	glEnableVertexAttribArray(0);//type
	glEnableVertexAttribArray(1);//position
	glEnableVertexAttribArray(2);//velocity
	glEnableVertexAttribArray(3);//age
	glEnableVertexAttribArray(4);//alpha
	glEnableVertexAttribArray(5);//size
	glEnableVertexAttribArray(6);//lifeSpan
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid*)4);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid*)16);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid*)28);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid*)32);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid*)36);
	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid*)40);
	glBeginTransformFeedback(GL_POINTS);
	if (flag)
	{
		glDrawArrays(GL_POINTS, 0, particleNum);
		flag = false;
	}
	else {
		glDrawTransformFeedback(GL_POINTS, transformFeedbacks[currVB]);
	}
	glEndTransformFeedback();
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);
	glDisable(GL_RASTERIZER_DISCARD);
}

void ParticleSystem::renderParticles(glm::mat4& worldMatrix,
	glm::mat4& viewMatrix, glm::mat4& projectMatrix)
{
	//glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	renderShader->Use();
	renderShader->SetMat4("model", worldMatrix);
	renderShader->SetMat4("view", viewMatrix);
	renderShader->SetMat4("projection", projectMatrix);
	glBindVertexArray(particleArrays[currTFB]);
	glBindBuffer(GL_ARRAY_BUFFER, particleBuffers[currTFB]);
	glEnableVertexAttribArray(0);//position
	glEnableVertexAttribArray(1);//age
	glEnableVertexAttribArray(2);//alpha
	glEnableVertexAttribArray(3);//size
	glEnableVertexAttribArray(4);//lifeSpan
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid*)4);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid*)28);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid*)32);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid*)36);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(particle), (const GLvoid*)40);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glDrawTransformFeedback(GL_POINTS, transformFeedbacks[currTFB]);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);

	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_PROGRAM_POINT_SIZE);
	glDepthMask(GL_TRUE);
}


void ParticleSystem::initLocation(particle particles[], int nums)
{
	srand(time(NULL));
	int n = 10;
	float radius = 0.2f;
	for (int x = 0; x < nums; x++) {
		glm::vec3 record(0.0f);
		for (int y = 0; y < n; y++) {
			record.x += (2.0f*float(rand()) / float(RAND_MAX) - 1.0f);
			record.z += (2.0f*float(rand()) / float(RAND_MAX) - 1.0f);
		}
		record.x *= radius;
		record.z *= radius;
		record.y = 1.0f;
		particles[x].type = LAUNCHER;
		particles[x].position = record;
		particles[x].velocity = (MAX_VELOCITY-MIN_VELOCITY) * (float(rand()) / float(RAND_MAX)) + MIN_VELOCITY;
		particles[x].alpha = 1.0f;
		particles[x].size = PARTICLE_SIZE;
		particles[x].age = (MAX_AGE - MIN_AGE)*(float(rand()) / float(RAND_MAX)) + MIN_AGE;
		particles[x].lifeSpan = particles[x].age*(1.0*(float(rand()) / float(RAND_MAX)) + 0.5);
	}
}

void loadTexture(const char* fileName, unsigned int* texture) {
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, channel;
	unsigned char *data = stbi_load(fileName, &width, &height, &channel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}