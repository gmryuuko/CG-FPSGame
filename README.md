# CG-FPSGame

ZJU CG 2021

文档：`/doc`

## 注意

`lib/glfw3.lib` 是64位版本。

MSVC需要添加编译参数`/utf-8`

`Transform`类不能使用xyz不等的`scale`，否则需要修改法线部分。

## 如何编译

使用CMake或者支持CMake工程的IDE（例如Visual Studio 2019及以上）打开这个文件夹，然后编译。必须编译64位版本，因为`lib`只提供了64位版本的库。

## TODO

- [ ] 模型和光照
  - [x] 完善`Mesh`和`Model`
  - [x] 能加载`.obj`文件到`Model`类
  - [x] 能加载`.mtl`材质文件和贴图
  - [ ] 或许可以用库来扩展其他类型的模型
  - [ ] 找免费素材
  - [x] 设计`Light`类
  - [ ] 设计`Scene`，用xml文件保存和读取
    - 一个摄像机
    - 若干个光源
    - 一些模型
- [ ] graphic
  - [x] 能绘制
  - [x] 添加贴图
  - [x] 添加光照
  - [ ] 阴影
- [ ] game logic
  - [ ] ...
