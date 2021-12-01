# CG-FPSGame
ZJU CG 2021

文档：`/doc`

## 注意

`lib/glfw3.lib` 是64位版本。

文件编码是 `UTF-8 with BOM`，因为 MSVC 不认没有 BOM 的 utf8 编码，中文容易出问题（别写中文就不用管编码了）。

## 如何编译

使用CMake或者支持CMake工程的IDE（例如Visual Studio 2019及以上）打开这个文件夹，然后编译。必须编译64位版本，因为`lib`只提供了64位版本的库。