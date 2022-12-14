cmake_minimum_required(VERSION 3.5)

project(MetaParser)

# append cmake module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../CMake")

foreach(var ${CMAKE_MODULE_PATH})
	message(${var})
endforeach()

file(GLOB MetaParserSrc
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

source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${MetaParserSrc})

add_executable(MetaParser ${MetaParserSrc})

# add LLVM includes
target_include_directories(MetaParser PUBLIC ${LLVM_INCLUDE_DIRS} ".")

target_link_libraries(MetaParser ${LIBCLANG_LIBRARY})

target_precompile_headers(MetaParser PRIVATE "PreCompiled.h")

target_compile_features(MetaParser
PUBLIC cxx_std_17
)

# copy template mustache resources on post build
add_custom_command(TARGET MetaParser POST_BUILD
	# mustache templates directory
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_LIST_DIR}/../Resources"
		$<TARGET_FILE_DIR:MetaParser>	
)
