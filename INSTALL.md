##### Windows编译

###### vcpkg

```shell
git clone https://github.com/microsoft/vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
```

###### 工具链

CMake+TDM-GCC

MinGW的WIN32版本不支持std::thread，POSIX版本支持std::thread，但对

```shell
cmake -G "MinGW Makefiles" ..
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
mingw32-make.exe -j12

```