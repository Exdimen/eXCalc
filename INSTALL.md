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
mingw32-make.exe -j12

```



##### Linux编译（ubuntu）

###### vcpkg install

安装glfw3需要先安装

```shell
sudo apt-get install libx11-dev    
sudo apt install libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev pkg-config
sudo apt-get install build-essential libgtk-3-dev
```

根目录底下安装vcpkg

```shell
git clone https://github.com/microsoft/vcpkg
cd vcpkg
./bootstrap-vcpkg.sh
```

使用vcpkg清单模式配置依赖（vcpkg.json）

###### 编译代码

```shell
mkdir build
cd build
cmake ..
make
```

USB开启权限

```shell
sudo chmod 777 /dev/ttyUSB0
```

USB串口打不开，可能是brltty占用了串口

```shell
sudo dmesg | grep brltty
sudo apt remove brltty
```
