# Document

## Engine

目前实现了`FrameTime, Input, Graphic`，只需要调用`Graphic::CreateWindow`，`Time`和`Input`会自动初始化。

一个Render loop应当是这样：

```cpp
while (!Graphic::Closed()) {
    Graphic::SwapFrame();
    Graphic::Clear();
    // render
}
```

### FrameTime

```cpp
namespace FrameTime {
float GetTime();
float GetDeltaTime();
}
```

### Input

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

`keyCode`最大值应该是`348`（翻glfw头文件找的），最小值是`-1(UNKNOWN)`

`Input::GetKey(keyCode)`返回当前帧里`keyCode`有没有被按下。

`Input::GetKeyDown(keyCode)`只在按下按键的第一帧返回`true`

`Input::GetKeyUp(keyCode)`只在松开按键的第一帧返回`true`

#### Mouse

`GetCursor(double& xpos, double& ypos)`

鼠标移动事件不使用回调函数实现，鼠标快速移动的时候一帧里会调用很多次callback，实测对性能影响似乎还不小，掉了好多帧。

鼠标按键暂时还没写，可以集成到`GetKey()`函数里面吧

### Resource

管理所有资源，避免重复创建。

比如一个`Scene`里面重复使用了一个`Model`很多次，只有第一次读取，后来的直接使用。

```cpp
namespace Resource {
unordered_map<string, Texture*> textures;
unordered_map<string, Model*> models;
// unordered_map<string, Audio*> audio;
    
GetTexture(const string& path); // 图片文件路径
GetModel(const string& Model); // obj 文件路径
}

```

### Graphic

```cpp
class Shader;

namespace Graphic {
// 实现在一个render loop里的一次绘制
void Render(const Scene& scene);
}
```

### Gui

用dear imgui?

```cpp
namespace Gui {
    
}
```

## Game Object

为了复用相同的网格，Mesh类里没有Transform组件，而是在GameObject里有一个Transform。

每一个游戏里的对象都有一个独立的`Transform`，`Transform`之间构成树形结构

`GetModelMatrix()`获得对象的`model`矩阵

为了减小计算量，修改`Transform`属性之后不会立即计算`modelMatrix`，而是设置`dirty`属性。

当调用`GetModelMatrix()`时，如果`dirty=true`才会计算。

**现在GameObject下面直接放了一个Mesh，应该还是要改成Model**

### Transform

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

```cpp
class Camera
{
public:
    SetTransform();
    GetViewMatrix();
}
```

### Model

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
enum class LightType
{
    // 点光  方向光     聚光
    Point, Direction, Spot
};

class Light
{
public:
    Transform transform;
    
}
```

### GameObject

```cpp
class GameObject
{
    Transform *transform;
    Model* model;
}
```

### Scene

用xml保存场景？[leethomason/tinyxml2](https://github.com/leethomason/tinyxml2)

```cpp
class Scene
{
public:
    vector<Model*> models;
    vector<Camera*> cameras;
    vector<Light*> lights;
    Camera* mainCamera;
    
    int ReadScene(const string& path);
    int SaveScene(const string& path);
}
```
