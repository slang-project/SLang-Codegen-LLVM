CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

# #** Set compiler **
# set(CMAKE_C_COMPILER "/usr/bin/clang")
# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(SLangLLVM)

#** Version Setup **
SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")
SET(VERSION_PATCH "0")
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

#** Environment Setup **
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../executable)


#** Compilation Setup **
SET(CMAKE_CXX_FLAGS "-g3 -Wall -O3")
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/dependencies/include)
file(GLOB project_sources "${PROJECT_SOURCE_DIR}/src/*.cpp")

#** Compiler Output **
add_executable(slang_jtll ${project_sources})

#** Info Tab **
message("Binary Tree Path ${PROJECT_BINARY_DIR}")
message("Source Tree Path ${PROJECT_SOURCE_DIR}")
message("Executables Location ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message("Project Version " ${VERSION})
