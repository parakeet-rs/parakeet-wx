# Parakeet

[![Build (Linux x86_64)](https://github.com/jixunmoe/parakeet/actions/workflows/build-linux-x86_64.yml/badge.svg)](https://github.com/jixunmoe/parakeet/actions/workflows/build-linux-x86_64.yml) [![Build (Windows)](https://github.com/jixunmoe/parakeet/actions/workflows/build-windows.yml/badge.svg)](https://github.com/jixunmoe/parakeet/actions/workflows/build-windows.yml)

一个用于解密下载的音乐文件工具的桌面客户端，使用 wxWidgets 作为基础 GUI 库开发。

![主界面截图](.github/assets/app-main.png)

项目百科：[支持的格式][wiki_format] | [便携版制作][wiki_portable]

[wiki_format]: https://github.com/jixunmoe/parakeet/wiki/%E6%94%AF%E6%8C%81%E7%9A%84%E6%A0%BC%E5%BC%8F
[wiki_portable]: https://github.com/jixunmoe/parakeet/wiki/%E4%BE%BF%E6%90%BA%E7%89%88%E5%88%B6%E4%BD%9C

## 下载

你可以在 [Latest Release][dl_rel] 下载最新的稳定版。

[dl_rel]: https://github.com/jixunmoe/parakeet/releases/latest

随代码提交自动构建的文件可以在 GitHub Action 产生的文件内找到。

| 名称                | 链接                                            |
| :------------------ | :---------------------------------------------- |
| Windows (MSVS 2022) | [`win32.zip`][dl_win32] [`win64.zip`][dl_win64] |
| Ubuntu 22.04        | [`linux-x86_64.zip`][dl_linux_x86_64]           |
| Linux (AppImage)    | [`AppImage-x86_64.zip`][dl_appimage_x86_64]     |

[dl_win32]: https://nightly.link/jixunmoe/parakeet/workflows/build-windows/main/win32.zip
[dl_win64]: https://nightly.link/jixunmoe/parakeet/workflows/build-windows/main/win64.zip
[dl_linux_x86_64]: https://nightly.link/jixunmoe/parakeet/workflows/build-linux-x86_64/main/linux-x86_64.zip
[dl_appimage_x86_64]: https://nightly.link/jixunmoe/parakeet/workflows/build-linux-x86_64/main/AppImage-x86_64.zip

### 关于密钥

🚨 你需要自行寻找密钥或加密参数并填入到软件。该项目**不会**提供解密所需的密钥。

请自行逆向内容提供商的应用程序，或寻求他人的帮助。

## 构建说明

### Windows 下构建

- 安装 CMake
- 安装 Visual Studio 2022

```powershell
# 初始化 vcpkg
.\vcpkg\bootstrap-vcpkg.bat

 # 调试版
cmake --preset win64-2022-dbg
cmake --build --preset win64-2022-dbg

# 发布版
cmake --preset win64-2022-rel
cmake --build --preset win64-2022-rel
```

### Ubuntu 22.04 下构建

- 参考 `build-setup-ubuntu.sh` 的内容安装依赖的包。

```sh
# 初始化 CMake 构建
cmake -B build -G "Ninja Multi-Config"

# 编译/运行调试版
cmake --build build --config Debug
./build/Debug/parakeet_wx

# 编译/运行发布版
cmake --build build --config Release
./build/Release/parakeet_wx
```

或使用 CMake Presets:

```sh
# 初始化 CMake 构建
cmake --preset linux

# 编译/运行调试版
cmake --build --preset linux-dbg
./build/linux/Debug/parakeet_wx

# 编译/运行发布版
cmake --build --preset linux-rel
./build/linux/Release/parakeet_wx
```

## 相关内容与链接

### 由我开发或参与的项目

- [小鹦鹉桌面客户端 | Jixun 的项目集](https://jixun.moe/apps/parakeet/)
- [QMC v1 解密工具](https://jixun.moe/utils/qmc-v1-decode/)
  - GitHub: [C 实现](https://github.com/jixunmoe/qmc-decode)
- [QMC v2 解密工具](https://jixun.moe/utils/qmc-v2/)
  - GitHub: [C++ 实现](https://github.com/jixunmoe/qmc2) · [Rust 实现](https://github.com/jixunmoe/qmc2-rust)
- Joox 解密库 - JavaScript 实现 [unlock-music/joox-crypto](https://github.com/unlock-music/joox-crypto)
- 酷狗 KGM/VPR 解密 - JavaScript 实现 [jixunmoe/kugou-crypto](https://github.com/jixunmoe/kugou-crypto)

### 同类项目

该项目多多少少参考了下述项目所实现的解密算法：

- Unlock Music - [Web (TypeScript)](https://git.unlock-music.dev/um/web) · [Cli (Go)](https://github.com/unlock-music/cli)
- [nukemiko/takiyasha](https://github.com/nukemiko/takiyasha) - Python 的实现，支持多加密方案
- [davidxuang/MusicDecrypto](https://github.com/davidxuang/MusicDecrypto) - C# 的实现，支持多加密方案，使用 SIMD 加速。
- ncmdump & 衍生项目 - 原仓库已下线，参考了网易云音乐部分的解密代码。

该项目的灵感来自 Unlock Music 项目。

### 特别致谢

该项目的开发使用了下述代码库与工具：

- vcpkg 依赖管理
  - boost 库及其依赖；
  - Crypto++ 提供标准加解密算法；
  - rapid-json 提供配置文件解析与序列化；
  - wxWidgets 提供界面库及其依赖；
- CMake 提供方便的跨平台编译方案
- VSCode 与相关扩展进行开发

## 声明

> 我（小鹦鹉软件作者）不支持、亦不提倡盗版。
> 我认为人们能够选择如何享用购买的内容。
> 小鹦鹉软件的使用者应保留解密后的副本做个人使用，而非进行二次分发。
> 其它条款请参考软件自带的[授权协议][parakeet_lic]。
>
> I (the Parakeet author) do not endorse nor encourage piracy.
> I believe that people should have a choice when consuming purchased content.
> Parakeet users should keep their decrypted copies for private use, not to re-distribute them.
> For other terms, checkout the [license][parakeet_lic].

[parakeet_lic]: https://github.com/jixunmoe/parakeet/blob/main/LICENSE
