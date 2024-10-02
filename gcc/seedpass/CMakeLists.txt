cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_C_COMPILER "gcc-14" CACHE FILEPATH "" FORCE)
set(CMAKE_CXX_COMPILER "g++-14" CACHE FILEPATH "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

project(seedpass)

execute_process (COMMAND bash -c "${CMAKE_CXX_COMPILER} -print-file-name=plugin" OUTPUT_VARIABLE GCCPLUGINS_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
#message(STATUS "Стандартный каталог для размещения плагинов: ${GCCPLUGINS_DIR}")

include_directories(
    "${GCCPLUGINS_DIR}/include" 
    "/lib/gcc-cross/i686-linux-gnu/14/plugin/include"
)

set(CMAKE_SHARED_LIBRARY_PREFIX "gcc_")

add_library(${PROJECT_NAME} SHARED seedpass.cpp)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/../../rnd/plugins)
