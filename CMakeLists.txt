cmake_minimum_required(VERSION 2.8.11)
project(Piper)

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src/tokenizer)
add_subdirectory(src/parser)
add_subdirectory(src/logger)
add_subdirectory(src/runtime)

add_executable(piper src/piper.cpp)
target_link_libraries(piper LINK_PUBLIC Tokenizer)
target_link_libraries(piper LINK_PUBLIC Parser)
target_link_libraries(piper LINK_PUBLIC Logger)
target_link_libraries(piper LINK_PUBLIC Runtime)