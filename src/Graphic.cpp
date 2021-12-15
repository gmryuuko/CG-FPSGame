#include "Graphic.h"
#include "Input.h"
#include "FrameTime.h"
#include "Resource.h"

#include <iostream>

using std::string;
using namespace glm;

namespace Graphic {

void InitShader();
void InitShadowMapping();
void InitHdr();
void InitBloom();
void InitQuad();
void FrameBufferSizeCallback(GLFWwindow* window, int scrWidth, int scrHeight);

// Settings
// 阴影贴图尺寸
const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
// 窗口
GLFWwindow* window = nullptr;
// 窗口宽度
unsigned int scrWidth = 0;
// 窗口高度
unsigned int scrHeight = 0;
// 每轮绘制时清空缓存内容
unsigned int clearBit = 0;
// 清屏颜色
glm::vec4 clearColor = glm::vec4(0, 0, 0, 1);

// Shader
// 一般物体的Shader
Shader* mainShader = nullptr;
Shader* mainShader_v1 = nullptr;
Shader* mainShader_v2 = nullptr;
// 发光体Shader
Shader* lightShader = nullptr;
// 天空盒Shader
Shader* skyboxShader = nullptr;
// 深度贴图
Shader* depthShader = nullptr;
// HDR
Shader* hdrShader = nullptr;
// Debug用
Shader* texShader = nullptr;
// 高斯模糊
Shader* blurShader = nullptr;

// Shadow
// 深度帧缓冲
unsigned int depthMapFBO = 0;
// 深度贴图
unsigned int depthMap = 0;

// HDR
// 帧缓冲
unsigned int hdrFBO = 0;
// 颜色缓冲
unsigned int hdrBuffer[2] = { 0 };
// 启用HDR
bool hdrOn = false;
// 曝光度
float hdrExposure = 1.0;

// Bloom
bool bloomOn = false;
unsigned int blurFBO[2];
unsigned int blurBuffer[2];

//
unsigned int quadVao;

GLFWwindow* CreateWindow(const std::string& title, unsigned int scrWidth, unsigned int scrHeight) {
    
    Graphic::scrWidth = scrWidth;
    Graphic::scrHeight = scrHeight;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // create window
    window = glfwCreateWindow(scrWidth, scrHeight, title.c_str(), NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    // callback
    glfwSetFramebufferSizeCallback(window, &FrameBufferSizeCallback);
    std::cout << "debug" << std::endl;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load opengl function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    // depth test
    glEnable(GL_DEPTH_TEST);
    // cull back face
    glEnable(GL_CULL_FACE);
    // v sync
    glfwSwapInterval(1);

    // Init Input
    Input::SetWindow(window);
    // Init shader
    InitShader();
    // Init shadow map
    InitShadowMapping();
    // init hdr
    InitHdr();
    // bloom
    InitBloom();

    InitQuad();

    return window;
}

void FrameBufferSizeCallback(GLFWwindow* window, int scrWidth, int scrHeight) {

    Graphic::scrWidth = scrWidth;
    Graphic::scrHeight = scrHeight;

    glViewport(0, 0, scrWidth, scrHeight);
}

void SetCursorMode(int value) {
    if (window == nullptr) {
        return ;
    }
    glfwSetInputMode(window, GLFW_CURSOR, value);
}

int Closed() {
    if (window == nullptr) {
        return 1;
    }
    return glfwWindowShouldClose(window);
}

void Clear() {
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(clearBit);
}


void SetClear(unsigned int value) {
    clearBit |= value;
}
void RemoveClear(unsigned int value) {
    clearBit &= ~value;
}
void SetClearColor(glm::vec4 color) {
    clearColor = color;
}

void SwapFrame() {
    FrameTime::UpdateTime();
    
    // std::cout << "New Frame" << std::endl;

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void SetVSync(unsigned int value) {
    glfwSwapInterval(value);
}

void InitShader() {
    mainShader_v1 = new Shader("../shader/main.vert", "../shader/main.frag");
    mainShader_v2 = new Shader("../shader/main_v2.vert", "../shader/main_v2.frag");
    mainShader = mainShader_v2;
    lightShader = new Shader("../shader/light.vert", "../shader/light.frag");
    skyboxShader = new Shader("../shader/skybox.vert", "../shader/skybox.frag");
    texShader = new Shader("../shader/showTexture.vert", "../shader/showTexture.frag");
    depthShader = new Shader("../shader/depth.vert", "../shader/depth.frag");
    hdrShader = new Shader("../shader/hdr.vert", "../shader/hdr.frag");
    blurShader = new Shader("../shader/blur.vert", "../shader/blur.frag");
}

void InitQuad() {
    unsigned int quadVbo = 0;
    if (quadVao == 0) {
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        glad_glGenVertexArrays(1, &quadVao);
        glad_glGenBuffers(1, &quadVbo);
        glad_glBindVertexArray(quadVao);
        glad_glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
        glad_glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glad_glEnableVertexAttribArray(0);
        glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glad_glEnableVertexAttribArray(1);
        glad_glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }

}

void InitShadowMapping() {
    // 生成 深度帧缓冲 和 深度贴图
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    // set depth texture
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 把 depthMap 设置为 depthMapFBO 的深度缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void InitHdr() {
    // HDR frame buffer
    glad_glGenFramebuffers(1, &hdrFBO);
    glad_glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    // HDR color buffer
    glad_glGenTextures(2, hdrBuffer);
    for (int i = 0; i < 2; i++) {
        glad_glBindTexture(GL_TEXTURE_2D, hdrBuffer[i]);
        glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scrWidth, scrHeight, 0, GL_RGBA, GL_FLOAT, NULL);
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // add color buffer to hdrFBO
        glad_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, hdrBuffer[i], 0);
    }
    // depth buffer
    static unsigned int rboDepth;
    glad_glGenRenderbuffers(1, &rboDepth);
    glad_glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glad_glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, scrWidth, scrHeight);
    glad_glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    // 声明使用了两个color buffer
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glad_glDrawBuffers(2, attachments);

    // check
    if (glad_glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;

    glad_glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void InitBloom() {
    glad_glGenFramebuffers(2, blurFBO);
    glad_glGenTextures(2, blurBuffer);
    for (int i = 0; i < 2; i++) {
        glad_glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[i]);
        glad_glBindTexture(GL_TEXTURE_2D, blurBuffer[i]);
        glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scrWidth, scrHeight, 0, GL_RGBA, GL_FLOAT, NULL);
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glad_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurBuffer[i], 0);
        // check
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
    }
}

void RenderTexture(unsigned int texId) {
    static unsigned int tvao = 0;
    static unsigned int tvbo = 0;
    if (tvao == 0) {
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &tvao);
        glGenBuffers(1, &tvbo);
        glBindVertexArray(tvao);
        glBindBuffer(GL_ARRAY_BUFFER, tvbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    }

    texShader->Use();
    texShader->SetFloat("near_plane", 0);
    texShader->SetFloat("far_plane", 0); // TODO: 如果使用透视投影，这两个值就需要使用了
    texShader->SetInt("depthMap", 15);
    glActiveTexture(GL_TEXTURE15);
    glBindTexture(GL_TEXTURE_2D, texId);

    glad_glBindVertexArray(tvao);
    glad_glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glad_glBindVertexArray(0);
}

mat4 RenderDirLightShadow(Scene& scene, Light::DirLight& light, unsigned int texId) {

    texShader->Use();
    texShader->SetInt("depthMap", 8);
    mat4 lightProjection, lightView, lightSpaceMatrix;
    float nearPlane = 0.1, farPlane = 100;
    lightProjection = ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
    vec3 cameraPos = scene.mainCamera->transform->GetPosition();
    // 方向光照的阴影，只计算摄像机周围的部分空间
    lightView = lookAt(cameraPos - normalize(light.direction) * vec3(10.0f),
                       cameraPos, vec3(0, 1, 0));
    lightSpaceMatrix = lightProjection * lightView;
    // render depth map
    depthShader->Use();
    depthShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glad_glCullFace(GL_FRONT);
    for (auto& obj : scene.gameObjects) {
        // 光源不会有阴影吧
        obj->Draw(*depthShader, *depthShader, false);
    }
    glad_glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, scrWidth, scrHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return lightSpaceMatrix;
}

void RenderHdr() {
    glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    hdrShader->Use();
    hdrShader->SetInt("hdrBuffer", 0);
    glad_glActiveTexture(GL_TEXTURE0);
    glad_glBindTexture(GL_TEXTURE_2D, hdrBuffer[0]);
    hdrShader->SetInt("brightBuffer", 1);
    glad_glActiveTexture(GL_TEXTURE1);
    glad_glBindTexture(GL_TEXTURE_2D, bloomOn ? blurBuffer[0] : hdrBuffer[1]);

    glad_glBindVertexArray(quadVao);
    glad_glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void RenderBloom() {
    bool horizontal = true, first = true;
    int amount = 10;
    blurShader->Use();
    blurShader->SetInt("image", 0);
    glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (int i = 0; i < amount; i++) {
        glad_glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[horizontal]);
        glad_glClear(GL_COLOR_BUFFER_BIT);
        blurShader->SetInt("horizontal", horizontal);
        glad_glActiveTexture(GL_TEXTURE0);
        glad_glBindTexture(GL_TEXTURE_2D, first ? hdrBuffer[1] : blurBuffer[!horizontal]);

        glad_glBindVertexArray(quadVao);
        glad_glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glad_glBindVertexArray(0);

        horizontal = !horizontal;
        first = false;
    }
    glad_glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderScene(Scene& scene) {
    if (scene.mainCamera == nullptr) {
        std::cerr << "No camera." << std::endl;
        return;
    }

    // shadow
    // TODO: 目前只为第一个dirLight创建阴影
    if (scene.dirLights.empty()) {
        std::cerr << "No direction light." << std::endl;
        return;
    }
    mat4 lightSpaceMatrix;
    lightSpaceMatrix = RenderDirLightShadow(scene, *scene.dirLights[0], depthMap);

    // show depth map
    // RenderTexture(depthMap);
    // return;

    glad_glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set camera
    mat4 projection = perspective(glm::radians(scene.mainCamera->zoom), (float)scrWidth / (float)scrHeight, 0.1f, 100.0f);
    mainShader->Use();
    mainShader->SetMat4(UNIFORM_PROJECTION_MATRIX, projection);
    mainShader->SetMat4(UNIFORM_VIEW_MATRIX, scene.mainCamera->GetViewMatrix());
    mainShader->SetVec3("viewPos", scene.mainCamera->transform->GetPosition());
    lightShader->Use();
    lightShader->SetMat4(UNIFORM_PROJECTION_MATRIX, projection);
    lightShader->SetMat4(UNIFORM_VIEW_MATRIX, scene.mainCamera->GetViewMatrix());

    // set light
    mainShader->Use();
    mainShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
    mainShader->SetInt("nPointLights", scene.pointLights.size());
    for (int i = 0; i < scene.pointLights.size(); i++) {
        Light::SetPointLight(*mainShader, i, scene.pointLights[i]);
    }
    mainShader->SetInt("nDirLights", scene.dirLights.size());
    for (int i = 0; i < scene.dirLights.size(); i++) {
        Light::SetDirLight(*mainShader, i, scene.dirLights[i]);
    }

    // set shadow
    mainShader->Use();
    // mainShader->SetMat4("lightSpaceMatrix", )
    mainShader->SetInt("depthMap", 8);
    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    // render object
    for (auto& obj : scene.gameObjects) {
        obj->Draw(*mainShader, *lightShader);
        // obj->Test();
    }

    // render skybox
    if (scene.skybox != nullptr) {
        skyboxShader->Use();
        skyboxShader->SetMat4(UNIFORM_VIEW_MATRIX, mat4(mat3(scene.mainCamera->GetViewMatrix()))); // 消除位移
        skyboxShader->SetMat4(UNIFORM_PROJECTION_MATRIX, projection);
        scene.skybox->Draw(*skyboxShader);
    }

    glad_glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (bloomOn) {
        RenderBloom();
    }

    RenderHdr();
}

void ProcessInput() {
    if (Input::GetKeyUp(GLFW_KEY_ESCAPE)) {
        std::cout << "ESC up" << std::endl;
        glfwSetWindowShouldClose(window, true);
    }
    if (Input::GetKeyDown(GLFW_KEY_N)) {
        if (mainShader == mainShader_v1) {
            mainShader = mainShader_v2;
            std::cout << "Normal texture enable." << std::endl;
        } else {
            mainShader = mainShader_v1;
            std::cout << "Normal texture disable." << std::endl;
        }
    }
    if (Input::GetKeyDown(GLFW_KEY_H)) {
        hdrShader->Use();
        hdrOn = !hdrOn;
        hdrShader->SetBool("hdrOn", hdrOn);
        hdrShader->SetFloat("exposure", hdrExposure);
        std::cout << "HDR " << (hdrOn ? "on" : "off") << ", exposure: " << hdrExposure << std::endl;
    }
    if (Input::GetKeyDown(GLFW_KEY_B)) {
        hdrShader->Use();
        bloomOn = !bloomOn;
        hdrShader->SetBool("bloomOn", bloomOn);
        std::cout << "Bloom " << (bloomOn ? "on" : "off") << "." << std::endl;
    }
}


} // namespace 