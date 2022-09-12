cmake_minimum_required(VERSION 3.5)

project(Test)

# executable MetaParser
add_subdirectory(../MetaParser MetaParser)

#append cmake module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cmake")

#include the meta parser cmake script
include(MetaParser)

#------reflection module------
set(TEST_META_SOURCE_ROOT "${CMAKE_CURRENT_LIST_DIR}")
set(TEST_META_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/Generated")

set(TEST_META_GENERATED_HEADERS "")
set(TEST_META_GENERATED_SOURCES "")

set(TEST_REFLECTION_HEADER "Header/Reflection.h")
set(TEST_MODULE_HEADER "Header/ReflectionModule.h")
set(TEST_MODULE_TARGET "TestModule")

set(TEST_HEADERS "Header/Range.h") # important, this prefix

set(INCLUDES "")

#just get the generated file name
meta_parser_prebuild(
    TARGET ${TEST_MODULE_TARGET} # module target
    GENERATED_DIR ${TEST_META_GENERATED_DIR}# generated directory
    SOURCE_ROOT ${TEST_META_SOURCE_ROOT}# source root
    HEADER_FILES ${TEST_HEADERS} # need parsed header
    MODULE_HEADER ${TEST_MODULE_HEADER} # module header
    OUT_MODULE_SOURCE META_MODULE_SOURCE # meta module source
    OUT_GENERATED_FILES META_GENERATED_FILES # meta module files
    OUT_INC TEST_META_GENERATED_HEADERS # out reference variable
    OUT_SRC TEST_META_GENERATED_SOURCES # out reference variable
)

#message(${META_MODULE_SOURCE})
#
#foreach(GENERATED_FILE ${META_GENERATED_FILES})
#    message(${GENERATED_FILE})    
#endforeach()
#
#foreach(GENERATED_HEADER ${TEST_META_GENERATED_HEADERS})
#    message(${GENERATED_HEADER})
#endforeach()
#
#foreach(GENERATED_SOURCE ${TEST_META_GENERATED_SOURCES})
#    message(${GENERATED_SOURCE})
#endforeach()

#get a reflection module target
add_library(${TEST_MODULE_TARGET} ${META_GENERATED_FILES})

# message("${TEST_META_GENERATED_DIR}/PreCompiled.h")

target_precompile_headers(${TEST_MODULE_TARGET} PRIVATE "${TEST_META_GENERATED_DIR}/PreCompiled.h")

target_include_directories(${TEST_MODULE_TARGET} PRIVATE "${TEST_META_GENERATED_DIR}")

#------reflection module------

file(GLOB TestSrc
"main.cpp"
"Header/*.h"
"Source/*.cpp"
"PreCompiled.h")

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${TestSrc})

add_executable(Test ${TestSrc})

# link the reflection module target
target_link_libraries(Test ${TEST_MODULE_TARGET})

# include the reflecton module includes
# target_include_directories(Test ${TEST_META_GENERATED_HEADERS})

target_compile_features(Test
PUBLIC cxx_std_17
)

target_include_directories(Test PUBLIC ".")

target_precompile_headers(Test PRIVATE "PreCompiled.h")

meta_parser_build(
    TARGET ${TEST_MODULE_TARGET} # target name
    SOURCE_ROOT ${TEST_META_SOURCE_ROOT} # source root
    GENERATED_DIR ${TEST_META_GENERATED_DIR} # generated directory
    GENERATED_FILES ${META_GENERATED_FILES} # generated files
    SOURCE_FILE ${TEST_REFLECTION_HEADER} # reflection header
    MODULE_HEADER ${TEST_MODULE_HEADER} # module header
    MODULE_SOURCE_FILE ${META_MODULE_SOURCE} # module source
    HEADER_FILES ${TEST_HEADERS} # depend header files
    INCLUDES ${INCLUDES} # -I parameter
    PCH_NAME "PreCompiled"
    PARSER_EXECUTABLE "$<TARGET_FILE:MetaParser>"
)
