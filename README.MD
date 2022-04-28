# um-desktop

## Windows 下构建

- 安装 CMake
- 安装 Visual Studio 2019+

```powershell
cmake --preset win64-2019
cmake --build --preset win64-2019-dbg # 调试版
cmake --build --preset win64-2019-rel # 发布版
```
