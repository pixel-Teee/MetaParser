cmake_minimum_required(VERSION 3.5)

project(MetaParser)

add_subdirectory(Core)

target_compile_features(Core
PUBLIC cxx_std_17
)