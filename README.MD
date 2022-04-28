# um-desktop

## Windows 下构建

- 安装 CMake
- 安装 Visual Studio 2019+
- 安装并设定 vcpkg

注意将 `C:/vcpkg` 替换为你的 vcpkg 路径。

```powershell
$VCPKG="C:/vcpkg/vcpkg"

# 安装依赖
& "$VCPKG/vcpkg" install wxwidgets:x64-windows-static

# 准备
rm -Recursive build && mkdir build && cd build

# 构建
cmake `
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG/scripts/buildsystems/vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=x64-windows-static `
  ..
cmake --build . --config Release
```
