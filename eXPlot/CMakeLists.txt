# CMakeList.txt: eXPlot 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)


find_package(imgui CONFIG REQUIRED)
# find_package(implot CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(kissfft CONFIG REQUIRED COMPONENT float)
find_package(CSerialPort CONFIG REQUIRED)

# implot

file(GLOB IMPLOT_SOURCE_FILES "3rdparty/implot/*.cpp")
add_library(implot SHARED ${IMPLOT_SOURCE_FILES})
target_include_directories(implot PUBLIC "3rdparty/implot")
# sigslot
# find_path(SIGSLOT_INCLUDE_DIRS "sigslot.h")

# folly
find_package(folly CONFIG REQUIRED)

add_subdirectory(3rdparty/nativefiledialog-extended)
add_subdirectory(3rdparty/glad)

include_directories("src")
include_directories("src/common")
include_directories("src/Connectivity")
include_directories("src/TransportLayer")
include_directories("src/SystemLib")
include_directories("3rdparty")
include_directories("3rdparty/imgui_backends")
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
	"src/common/Fonts/*.cpp"
	"3rdparty/imgui_backends/*.cpp")



# 将源代码添加到此项目的可执行文件。
add_executable (eXPlot ${SRC_FILES})

target_compile_features(eXPlot PRIVATE cxx_std_17)

target_link_libraries(eXPlot PRIVATE implot)
target_link_libraries(eXPlot PRIVATE nfd)
target_link_libraries(eXPlot PRIVATE imgui::imgui)
target_link_libraries(eXPlot PRIVATE glfw glad)

# target_link_libraries(eXPlot PRIVATE implot::implot)
# target_link_libraries(eXPlot PRIVATE GLEW::GLEW)
target_link_libraries(eXPlot PRIVATE kissfft::kissfft)
target_link_libraries(eXPlot PRIVATE Folly::folly Folly::folly_deps Folly::follybenchmark Folly::folly_test_util)
target_link_libraries (eXPlot PUBLIC ${CSerialPort_LIBRARY})
# target_include_directories(eXPlot PRIVATE ${SIGSLOT_INCLUDE_DIRS})
