# CMakeList.txt: eXPlot 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)


include_directories("src")

include_directories(${CSerialPort_INCLUDE_DIR})

file(GLOB SRC_FILES 
	"src/*.h" 
	"src/*.cpp")



# 将源代码添加到此项目的可执行文件。
add_executable (eXCalc ${SRC_FILES})


if (CMAKE_HOST_WIN32)
#	target_link_libraries(Calc PRIVATE mingw_stdthreads)
endif()