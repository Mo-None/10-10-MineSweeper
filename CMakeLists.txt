cmake_minimum_required(VERSION 3.31)
project(minesweeper)

set(CMAKE_CXX_STANDARD 20)
link_directories("./EasyX_26.1.1/lib/VC2015/x64")

add_executable(minesweeper main.cpp
)
target_link_libraries(minesweeper EasyXa.lib)