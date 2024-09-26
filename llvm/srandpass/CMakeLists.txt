cmake_minimum_required(VERSION 3.20.0)

set(BIN_DIR "/home/dev/llvm20/bin" CACHE FILEPATH "" FORCE)
set(CMAKE_C_COMPILER "${BIN_DIR}/clang" CACHE FILEPATH "" FORCE)
set(CMAKE_CXX_COMPILER "${BIN_DIR}/clang++" CACHE FILEPATH "" FORCE)

project(srandpass)

include_directories(
    "~/llvm20/include/" 
)

set(CMAKE_SHARED_LIBRARY_PREFIX "llvm_")

add_library(${PROJECT_NAME} SHARED srandpass.cpp)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/../../srand/plugins)
