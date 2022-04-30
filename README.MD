# um-desktop

## Windows 下构建

- 安装 CMake
- 安装 Visual Studio 2019

```powershell
# 初始化 vcpkg
.\vcpkg\bootstrap-vcpkg.bat

 # 调试版
cmake --preset win64-2019-dbg
cmake --build --preset win64-2019-dbg

# 发布版
cmake --preset win64-2019-rel
cmake --build --preset win64-2019-rel
```

## Linux (Ubuntu 22.04) 下构建

- 安装工具链： `sudo apt-get install build-essential cmake ninja-build`
- 安装下述依赖：

```sh
sudo apt-get install \
  libgcrypt20-dev \
  libgtk-3-dev \
  libsecret-1-dev \
  libsystemd-dev \
  libwebp-dev \
  libzstd-dev \
  pkg-config
```
