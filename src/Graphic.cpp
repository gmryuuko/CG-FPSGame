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
void FrameBufferSizeCallback(GLFWwindow* window, int scrWidth, int scrHeight);

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
// 一般物体的Shader
Shader* mainShader = nullptr;
// 发光体Shader
Shader* lightShader = nullptr;
// 天空盒Shader
Shader* skyboxShader = nullptr;
// 深度贴图
Shader* depthShader = nullptr;
// Debug用
Shader* texShader = nullptr;
// 深度帧缓冲
unsigned int depthMapFBO = 0;
// 深度贴图
unsigned int depthMap = 0;


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

    if (Input::GetKeyUp(GLFW_KEY_ESCAPE)) {
        std::cout << "ESC up" << std::endl;
        glfwSetWindowShouldClose(window, true);
    }
    if (Input::GetKeyDown(GLFW_KEY_SPACE)) {
        std::cout << "Space down" << std::endl;
    }
    if (Input::GetKey(GLFW_KEY_SPACE)) {
        std::cout << "Space pressing" << std::endl;
    }
    if (Input::GetKeyUp(GLFW_KEY_SPACE)) {
        std::cout << "Space up" << std::endl;
    }
}

void SetVSync(unsigned int value) {
    glfwSwapInterval(value);
}

void InitShader() {
    mainShader = new Shader("../shader/main.vert", "../shader/main.frag");
    lightShader = new Shader("../shader/light.vert", "../shader/light.frag");
    skyboxShader = new Shader("../shader/skybox.vert", "../shader/skybox.frag");
    texShader = new Shader("../shader/showTexture.vert", "../shader/showTexture.frag");
    depthShader = new Shader("../shader/depth.vert", "../shader/depth.frag");
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

void RenderDirLightShadow(Scene& scene, Light::DirLight& light, unsigned int texId) {

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
    for (auto& obj : scene.gameObjects) {
        // 光源不会有阴影吧
        obj->Draw(*depthShader, *depthShader, false);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, scrWidth, scrHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // TODO: 这个放到外面
    mainShader->Use();
    mainShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
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
    RenderDirLightShadow(scene, *scene.dirLights[0], depthMap);

    // show depth map
    // RenderTexture(depthMap);
    // return;

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
}

} // namespace 