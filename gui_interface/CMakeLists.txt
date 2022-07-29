project(VPI_GUI C CXX)
cmake_minimum_required(VERSION 3.3.2)

include(cmake/App.cmake)

set(SOURCES "src/MyApp.hpp"
            "src/MyApp.cpp"
            "src/InteropFunctions.hpp"
            "src/InteropFunctions.cpp"
            "src/main.cpp")

add_app("${SOURCES}")