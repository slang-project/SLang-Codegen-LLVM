CMAKE_MINIMUM_REQUIRED(VERSION 3.0)


#** Set compiler **
# SET(CMAKE_C_COMPILER "/usr/bin/clang")
# SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")

SET(CMAKE_C_COMPILER "/usr/bin/gcc")
SET(CMAKE_CXX_COMPILER "/usr/bin/g++")

# ^ COMMENT BOTH FOR SYSTEM TO FIND APPROPRIATE COMPILER, E.G. ON WINDOWS

project(SLangLLVM)

#** Version Setup **
SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")
SET(VERSION_PATCH "1")

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

#** Environment Setup **
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../executable)


#** Colored Errors Output **
option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()


#** Discover LLVM Build Flags in Current System **
SET(LLVM_FLAGS_CALL "llvm-config-7")
SET(LLVM_CXXFLAGS_CALL_FLAGS "--cxxflags")
SET(LLVM_LIBSFLAGS_CALL_FLAGS "--libs")
SET(LLVM_lDFLAGS_CALL_FLAGS "--ldflags")

if(UNIX)
   execute_process(COMMAND ${LLVM_FLAGS_CALL} ${LLVM_CXXFLAGS_CALL_FLAGS} OUTPUT_VARIABLE LLVM_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
   execute_process(COMMAND ${LLVM_FLAGS_CALL} ${LLVM_LIBSFLAGS_CALL_FLAGS} OUTPUT_VARIABLE LLVM_LIB_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
   execute_process(COMMAND ${LLVM_FLAGS_CALL} ${LLVM_lDFLAGS_CALL_FLAGS} OUTPUT_VARIABLE LLVM_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
elseif(WIN32)
   execute_process(COMMAND CMD /c ${LLVM_FLAGS_CALL} ${LLVM_CXXFLAGS_CALL_FLAGS} OUTPUT_VARIABLE LLVM_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
   execute_process(COMMAND CMD /c ${LLVM_FLAGS_CALL} ${LLVM_LIBSFLAGS_CALL_FLAGS} OUTPUT_VARIABLE LLVM_LIB_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
   execute_process(COMMAND CMD /c ${LLVM_FLAGS_CALL} ${LLVM_lDFLAGS_CALL_FLAGS} OUTPUT_VARIABLE LLVM_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

#** Compilation Setup **
SET(CMAKE_CXX_FLAGS "-g3 ${LLVM_CXX_FLAGS} ${LLVM_LIB_FLAGS} ${LLVM_LD_FLAGS}")
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/dependencies/include)
file(GLOB project_sources "${PROJECT_SOURCE_DIR}/src/*.cpp")


#** Compiler Output **
add_executable(slang_jtll ${project_sources})


# #** Info Tab **
# message("Binary Tree Path ${PROJECT_BINARY_DIR}")
# message("Source Tree Path ${PROJECT_SOURCE_DIR}")
# message("Executables Location ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
# message("Project Version " ${VERSION})
