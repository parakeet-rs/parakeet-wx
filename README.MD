# um-desktop

## 构建说明

### Windows 下构建

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

### Linux (Ubuntu 22.04) 下构建 (vcpkg)

- 安装工具链： `sudo apt-get install build-essential cmake ninja-build`
- 安装下述构建依赖：

  ```sh
  sudo apt-get install \
    libgcrypt20-dev \
    libgtk-3-dev \
    libjpeg-dev \
    libpng-dev \
    libsecret-1-dev \
    libsystemd-dev \
    libtiff-dev \
    libwebp-dev \
    libzstd-dev \
    pkg-config
  ```

- 执行 `bash ./prepare.linux.sh` 进行初始化。

#### Linux (Ubuntu 22.04)

- 安装工具链： `sudo apt-get install build-essential cmake ninja-build`
- 安装下述构建依赖：

  ```sh
  sudo apt-get install \
    libboost-filesystem-dev \
    libboost-nowide-dev \
    libboost-thread-dev \
    libgcrypt20-dev \
    libgtk-3-dev \
    libjpeg-dev \
    libpng-dev \
    libssl-dev \
    libsecret-1-dev \
    libsystemd-dev \
    libtiff-dev \
    libwebp-dev \
    libzstd-dev \
    pkg-config
  ```
