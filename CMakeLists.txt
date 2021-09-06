# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("8080-emulator")

set(PROJECT_MODULE_NAME "8080-emulator")

file(GLOB_RECURSE PROJECT_SOURCE_FILES "./src/*.h" "./src/*.hpp" "./src/*.cpp")

include_directories(
	"${PROJECT_SOURCE_DIR}/vendors/SDL2/include"
)

add_executable(${PROJECT_MODULE_NAME} ${PROJECT_SOURCE_FILES})

target_link_libraries(${PROJECT_MODULE_NAME}
	${PROJECT_SOURCE_DIR}/vendors/SDL2/lib/x64/SDL2.lib
	${PROJECT_SOURCE_DIR}/vendors/SDL2/lib/x64/SDL2_ttf.lib
)
