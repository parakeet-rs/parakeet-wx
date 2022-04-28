Build in Windows:

```cmd
C:/vcpkg/vcpkg install wxwidgets:x64-windows
C:/vcpkg/vcpkg install wxwidgets:x64-windows-static

mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-static  ..
cmake --build . --config Release
```

Problem: Static build does not work. Unresolved externals.
