# um-desktop

## Windows 下构建

- 安装 CMake
- 安装 Visual Studio 2019+

```powershell
$VCPKG_ROOT="./vcpkg"
$VCPKG_ROOT = (Resolve-Path "$VCPKG_ROOT").Path

# 初次设定
& "$VCPKG_ROOT/bootstrap-vcpkg.bat"

# 安装依赖
& "$VCPKG_ROOT/vcpkg" install --feature-flags=manifests --triplet x64-windows-static

# 准备
if (Test-Path build) { Remove-Item -Recurse -Force build }
New-Item -Type Directory build
cd build

# 构建
cmake `
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=x64-windows-static `
  ..
cmake --build . --config Release
```
