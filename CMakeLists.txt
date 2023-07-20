# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

if(CMAKE_HOST_WIN32)
    set(CMAKE_GENERATOR "MinGW Makefiles")
endif()

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")


   
project ("eXPlot")

# 包含子项目。
add_subdirectory ("eXPlot")
