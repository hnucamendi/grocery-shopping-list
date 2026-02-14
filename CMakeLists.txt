cmake_minimum_required(VERSION 3.16)

project(GroceryShoppingList)

include(FetchContent)

set_property(GLOBAL PROPERTY CMAKE_CXX_STANDARD C++17)

add_compile_options(-Wall -Wextra)

find_package(wxWidgets REQUIRED COMPONENTS core base)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_Declare(catch GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG 88abf9bf325c798c33f54f6b9220ef885b267f4f)
FetchContent_MakeAvailable(json catch)

add_executable(grocery-shopping-list
    src/app/App.cpp
    src/ui/MainFrame.cpp
)
target_include_directories(grocery-shopping-list PRIVATE src)
target_link_libraries(grocery-shopping-list PRIVATE
    ${wxWidgets_LIBRARIES}
    nlohmann_json::nlohmann_json
)

add_executable(grocery-shopping-list-tests
    tests/domain/Placeholder_test.cpp
)
target_include_directories(grocery-shopping-list-tests PRIVATE src)
target_link_libraries(grocery-shopping-list-tests PRIVATE
    Catch2::Catch2WithMain
    nlohmann_json::nlohmann_json
)
