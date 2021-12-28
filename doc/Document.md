# Document

（不完全文档）

## Engine

### ObjLoader

使用`ObjLoader::LoadModel`加载模型，传入参数为模型的路径。在读取`.obj`文件的同时，还会检测是否存在`.mtl`文件描述材质，如果有的话一起读取并且应用材质，如果没有就使用默认材质。

使用`ObjLoader::WriteModel`导出材质，默认导出到相对路径`../export.obj`。

```cpp
namespace ObjLoader {
    Model* LoadModel(const std::string& path);
    bool WriteModel(const Model& model);
}
```

### FrameTime

帧时间相关函数封装

```cpp
namespace FrameTime {
    float GetTime();
    float GetDeltaTime();
}
```

### Input

键盘、鼠标输入相关函数封装

```cpp
namespace Input {
bool GetKey(int keyCode);
bool GetKeyUp(int keyCode);
bool GetKeyDown(int keyCode);
void GetCursor(double& xpos, double& ypos);
}
```

#### Keyboard

对于键盘按键，可能有三种使用方式

- 按下时触发一次
- 松开时触发一次
- 只关注是否按下

但是`glfwGetKey()`这个函数只返回是否按下，对于按下触发和松开触发需要注册回调函数。

`Input::GetKey(keyCode)`返回当前帧里`keyCode`有没有被按下。

`Input::GetKeyDown(keyCode)`只在按下按键的第一帧返回`true`

`Input::GetKeyUp(keyCode)`只在松开按键的第一帧返回`true`

#### Mouse

`GetCursor(double& xpos, double& ypos)`

鼠标移动事件不使用回调函数实现，鼠标快速移动的时候一帧里会调用很多次callback，实测对性能影响似乎还不小，掉了好多帧。

### Resource

管理所有资源，避免重复创建。

比如一个`Scene`里面重复使用了一个`Model`很多次，只有第一次读取，后来的直接使用。

```cpp
namespace Resource {
    unordered_map<string, Texture*> textures;
    unordered_map<string, Model*> models;
        
    GetTexture(const string& path); // 图片文件路径
    GetModel(const string& Model); // obj 文件路径
}

```

### Graphic

将`Scene`类的一个对象交给`Graphic::RenderScene()`，即可完成一帧内的渲染。

```cpp
namespace Graphic {
    // 实现在一个render loop里的一次绘制
    void RenderScene(const Scene& scene);
}
```

## Game Object

为了复用相同的网格，Mesh类里没有Transform组件，而是在GameObject里有一个Transform。

每一个游戏里的对象都有一个独立的`Transform`，`Transform`之间构成树形结构

`GetModelMatrix()`获得对象的`model`矩阵

为了减小计算量，修改`Transform`属性之后不会立即计算`modelMatrix`，而是设置`dirty`属性。

当调用`GetModelMatrix()`时，如果`dirty=true`才会计算。

### Transform

支持Translate、Rotate和Scale三种变换。

```cpp
class Transform 
{
public:
    Transform();
    vec3 position, scale, rotation;
    mat4 GetModelMatrix();
    vec3 GetAxisX();
    vec3 GetAxisY();
    vec3 GetAxisZ();
    vec3 GetPosition();
    vec3 GetRotation();
    vec3 GetScale();
}
```

### Camera

摄像机类。

```cpp
class Camera
{
public:
    SetTransform();
    GetViewMatrix();
}
```

### Model

一个模型（Model）下可以有多个网格（Mesh），绘制一个模型只需要调用`Model::Draw(shader)`函数即可。其中参数`shader`传入绘制此模型使用的着色器。

```cpp
struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texCoords;
    vec3 tangent;
    vec3 bitangent;
}

class Texture
{
public:
    unsigned int id;
    string path;
};

class Mesh
{
public:
    vector<Vertex>
}

class Model
{
private:
    vector<Texture*> textures;
    vector<Mesh*> meshes;
    string path;
    
}
```

### Light

```cpp
namespace Light {
    struct PointLight;
    struct DirLight;
    struct SpotLight;
}
```

### GameObject

游戏对象，主要由一个`Model`和一个`Transfrom`构成。

```cpp
class GameObject
{
    Transform *transform;
    Model* model;
}
```

### Scene

保存一个游戏场景，场景内主要有一个主摄像机，若干个游戏对象和若干个光源。

```cpp
class Scene
{
public:
    vector<GameObject*> gameObjects;
    vector<Camera*> cameras;
    vector<Light*> lights;
    Camera* mainCamera;
    
    int ReadScene(const string& path);
    int SaveScene(const string& path);
}
```
