# Doc

## Engine

这5个类应该都实现成静态类。

目前实现了`Time, Input, Graphic`，只需要调用`Graphic::CreateWindow`，`Time`和`Input`会自动初始化。

~~UpdateTime必须放在Render loop头或者尾，因为Input的检查和当前帧时间有关，如果在帧中间UpdateTime，Input会把前后当作两帧处理。~~

把`UpdateTime`放在`Graphic::SwapBuffer`里面实现了，一个Render loop应当是这样：

```cpp
while (!Graphic::Closed()) {
    Graphic::SwapFrame();
    Graphic::Clear();
    // render
}
```



### Time

```cpp
class Time
{
public:
    float deltaTime, time;
}
```



### Input

```cpp
class Input
{
public:
    GetKey();
    GetKeyDown();
    GetKeyUp();
}
```
#### Keyboard

对于键盘按键，可能有三种使用方式

- 按下时触发一次
- 松开时触发一次
- 只关注是否按下

但是`glfwGetKey()`这个函数只返回是否按下，对于按下触发和松开触发需要注册回调函数。

`keyCode`最大值应该是`348`，最小值是`-1(UNKNOWN)`

`Input::GetKey(keyCode)`返回当前帧里`keyCode`有没有被按下。

`Input::GetKeyDown(keyCode)`只在按下按键的第一帧返回`true`

`Input::GetKeyUp(keyCode)`只在松开按键的第一帧返回`true`

#### Mouse

鼠标移动事件不使用回调函数实现，鼠标快速移动的时候一帧里会调用很多次callback，实测对性能影响还不小？掉了好多帧。



### Resource

管理所有资源，避免重复创建。

比如一个Scene里面重复使用了一个Model很多次，只有第一次读取，后来的直接使用。

```cpp
class Resource
{
private:
    unordered_map<string, Texture*> textures;
    unordered_map<string, Model*> models;
    unordered_map<string, Audio*> audio;
    
public:
    GetTexture(const string& path);
    GetModel(const string& Model);
}
```



### Graphic

```cpp
class Shader {};

class Graphic
{
public:
    void Render(const Scene& scene);
}
```



### Gui

用dear imgui？？

```cpp
class Gui
{
    
}
```





## Game Object

为了复用相同的网格，Mesh类里没有Transform组件，而是在GameObject里有一个Transform。

每一个游戏里的对象都有一个独立的`Transform`，`Transform`之间构成树形结构

`GetModelMatrix()`获得对象的`model`矩阵

为了减小计算量，修改`Transform`属性之后不会立即计算`modelMatrix`，而是设置`dirty`属性。

当调用`GetModelMatrix()`时，如果`dirty=true`才会计算。

**TODO: 目前GameObject下面直接放了一个Mesh，应该还是要改成Model**

### Transform

```cpp
class Transform 
{
public:
    Transform();
    vec3 position, scale, rotation;
    mat4 GetModelMatrix() const;
}
```



### Camera

```cpp
class Camera
{
public:
    GetViewMatrix();
}
```



### Model

```cpp
class Texture
{
public:
    unsigned int id;
    string path;
};

class Mesh
{
public:
    vector<Vertex
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
    Camera *camera;
    Model *model;
    Light *light;
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