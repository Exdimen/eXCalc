# CMakeList.txt: eXPlot 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)


# find_package(imgui CONFIG REQUIRED)
# find_package(implot CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(kissfft CONFIG REQUIRED COMPONENT float)
# find_package(CSerialPort CONFIG REQUIRED)

#imgui
file(GLOB IMGUI_SOURCE_FILES 	"3rdparty/imgui/*.cpp"
								"3rdparty/imgui/backends/glfw.cpp"
								"3rdparty/imgui/backends/opengl3.cpp"
								"3rdparty/imgui/backends/imgui_impl_glfw.cpp"
								"3rdparty/imgui/backends/imgui_impl_opengl3.cpp")

add_library(imgui STATIC ${IMGUI_SOURCE_FILES})
target_include_directories(imgui PUBLIC "3rdparty/imgui/" "3rdparty/imgui/backends/")
target_link_libraries(imgui PRIVATE glfw glad)
link_libraries(imgui)	# 后面公开链接
# implot
file(GLOB IMPLOT_SOURCE_FILES "3rdparty/implot/*.cpp")
add_library(implot SHARED ${IMPLOT_SOURCE_FILES})
target_include_directories(implot PUBLIC "3rdparty/implot/")

# CSerialPort
set(CSerialPortRootPath "3rdparty/cserialport/")
include_directories(${CSerialPortRootPath}/include)
list(APPEND CSerialPortSourceFiles ${CSerialPortRootPath}/src/SerialPort.cpp ${CSerialPortRootPath}/src/SerialPortBase.cpp ${CSerialPortRootPath}/src/SerialPortInfo.cpp ${CSerialPortRootPath}/src/SerialPortInfoBase.cpp)
if (WIN32)
    list(APPEND CSerialPortSourceFiles ${CSerialPortRootPath}/src/SerialPortInfoWinBase.cpp ${CSerialPortRootPath}/src/SerialPortWinBase.cpp)
    list(APPEND CSerialPortSourceFiles ${CSerialPortRootPath}/lib/version.rc)
elseif (UNIX)
    list(APPEND CSerialPortSourceFiles ${CSerialPortRootPath}/src/SerialPortInfoUnixBase.cpp ${CSerialPortRootPath}/src/SerialPortUnixBase.cpp)
endif ()

add_library(cserialport SHARED ${CSerialPortSourceFiles})
if (WIN32)
	target_link_libraries(cserialport PRIVATE setupapi)
endif()

# sigslot
# find_path(SIGSLOT_INCLUDE_DIRS "sigslot.h")

# folly
# find_package(folly CONFIG REQUIRED)



add_subdirectory(3rdparty/nativefiledialog-extended)
add_subdirectory(3rdparty/glad)

include_directories("src")
include_directories("src/common")
include_directories("src/Connectivity")
include_directories("src/TransportLayer")
include_directories("src/SystemLib")
include_directories("3rdparty")

include_directories(${CSerialPort_INCLUDE_DIR})

file(GLOB SRC_FILES 
	"src/*.h" 
	"src/*.cpp"
	"src/Connectivity/*.h"
	"src/Connectivity/*.cpp"
	"src/Connectivity/*.c"
	"src/SystemLib/*.cpp"
	"src/TransportLayer/*.h"
	"src/TransportLayer/*.cpp" 
	"src/common/*.h"
	"src/common/*.cpp"
	"src/common/Fonts/*.h"
	"src/common/Fonts/*.cpp")



# 将源代码添加到此项目的可执行文件。
add_executable (eXPlot ${SRC_FILES})

target_compile_features(eXPlot PRIVATE cxx_std_17)

target_link_libraries(eXPlot PRIVATE implot)

target_link_libraries(eXPlot PRIVATE nfd)
# target_link_libraries(eXPlot PRIVATE imgui)

target_link_libraries(eXPlot PRIVATE glfw glad)
# target_link_libraries(eXPlot PRIVATE implot::implot)
# target_link_libraries(eXPlot PRIVATE GLEW::GLEW)
target_link_libraries(eXPlot PRIVATE kissfft::kissfft)
# target_link_libraries(eXPlot PRIVATE CSerialPort)
# target_link_libraries(eXPlot PRIVATE Folly::folly Folly::folly_deps Folly::follybenchmark Folly::folly_test_util)
target_link_libraries (eXPlot PRIVATE cserialport)
# target_include_directories(eXPlot PRIVATE ${SIGSLOT_INCLUDE_DIRS})

if (CMAKE_HOST_WIN32)
#	target_link_libraries(eXPlot PRIVATE mingw_stdthreads)
endif()