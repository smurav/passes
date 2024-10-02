cmake_minimum_required(VERSION 3.20.0)

set(EXECURABLE_NAME "rnd" CACHE FILEPATH "" FORCE)

if (USE_LLVM) # cmake -DUSE_LLVM=1
  set(EXECURABLE_NAME "${EXECURABLE_NAME}_llvm")
  set(CMAKE_C_COMPILER "~/llvm20/bin/clang" CACHE FILEPATH "" FORCE)
  set(CMAKE_CXX_COMPILER "~/llvm20/bin/clang++" CACHE FILEPATH "" FORCE)
  if (USE_PASS) # cmake -DUSE_PASS=1
    set(CMAKE_CXX_FLAGS "-fpass-plugin=${CMAKE_SOURCE_DIR}/plugins/llvm_seedpass.so")
  endif()
else()
  set(EXECURABLE_NAME "${EXECURABLE_NAME}_gcc")
  set(CMAKE_C_COMPILER "gcc-14" CACHE FILEPATH "" FORCE)
  set(CMAKE_CXX_COMPILER "g++-14" CACHE FILEPATH "" FORCE)
  if (USE_PASS) # cmake -DUSE_PASS=1
    set(CMAKE_CXX_FLAGS "-fdump-tree-optimized -fplugin=${CMAKE_SOURCE_DIR}/plugins/gcc_seedpass.so")
  endif()
endif()

if (USE_PASS) # cmake -DUSE_PASS=1
  set(EXECURABLE_NAME "${EXECURABLE_NAME}_instr")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE FILEPATH "" FORCE)

project(rnd)

add_executable(${PROJECT_NAME} rnd.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${EXECURABLE_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR})

