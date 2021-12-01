﻿# Doc

## Engine

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
}
```



- press触发
- release触发
- 按下时持续作用，每帧检测



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

每一个游戏里的对象都有一个独立的`Transform`，`Transform`之间构树形结构

`GetModelMatrix()`获得对象的`model`矩阵

为了减小计算量，修改`Transform`属性之后不会立即计算`modelMatrix`，而是设置`dirty`属性。

当调用`GetModelMatrix()`时，如果`dirty=true`才会计算。

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