cmake_minimum_required(VERSION 3.20)
project(wxhello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_executable(wxhello main.cpp)
target_link_libraries(wxhello PRIVATE ${wxWidgets_LIBRARIES})

