# um-desktop

## Windows 下构建

- 安装 CMake
- 安装 Visual Studio 2019

```powershell
 # 调试版
cmake --preset win64-2019-dbg
cmake --build --preset win64-2019-dbg

# 发布版
cmake --preset win64-2019-rel
cmake --build --preset win64-2019-rel
```
