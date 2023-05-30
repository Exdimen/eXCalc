# CMakeList.txt: eXPlot 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)


find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(unofficial-kissfft CONFIG REQUIRED)

add_subdirectory(3rdparty/nativefiledialog-extended)
add_subdirectory(3rdparty/glad)

include_directories("src")
include_directories("src/common")
include_directories("src/Connectivity")
include_directories("src/TransportLayer")
include_directories("3rdparty")
include_directories("3rdparty/imgui_backends")


file(GLOB SRC_FILES 
	"src/*.h" 
	"src/*.cpp"
	"src/Connectivity/*.h"
	"src/Connectivity/*.cpp" 
	"src/TransportLayer/*.h"
	"src/TransportLayer/*.cpp" 
	"src/common/*.h"
	"src/common/*.cpp"
	"src/common/Fonts/*.h"
	"src/common/Fonts/*.cpp"
	"3rdparty/imgui_backends/*.cpp")



# 将源代码添加到此项目的可执行文件。
add_executable (exPlot ${SRC_FILES})

target_compile_features(exPlot PRIVATE cxx_std_17)


target_link_libraries(exPlot PRIVATE nfd)
target_link_libraries(exPlot PRIVATE imgui::imgui)
target_link_libraries(exPlot PRIVATE glfw)
target_link_libraries(exPlot PRIVATE implot::implot)
# target_link_libraries(exPlot PRIVATE GLEW::GLEW)
target_link_libraries(exPlot PRIVATE unofficial::kissfft::kissfft)
target_link_libraries(exPlot PUBLIC glad)
