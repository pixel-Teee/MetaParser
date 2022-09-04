cmake_minimum_required(VERSION 3.5)

project(Core)

# append cmake module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

foreach(var ${CMAKE_MODULE_PATH})
	message(${var})
endforeach()

file(GLOB CoreSrc
"Mustache/mustache.hpp"
"main.cpp"
"Source/*.cpp"
"Header/*.h"
"Header/*.hpp"
"PreCompiled.h"
"PreCompiled.cpp"
"Header/ConstructorTokenSymbols.inl"
)

find_package(LLVM_Root REQUIRED)

source_group(TREE ${CMAKE_SOURCE_DIR}/Core FILES ${CoreSrc})

add_executable(Core ${CoreSrc})

# add LLVM includes
target_include_directories(Core PUBLIC ${LLVM_INCLUDE_DIRS} ".")

target_link_libraries(Core ${LIBCLANG_LIBRARY})

target_precompile_headers(Core PRIVATE "PreCompiled.h")

