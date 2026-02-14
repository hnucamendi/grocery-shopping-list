cmake_minimum_required(VERSION 3.16)

project(GroceryShoppingList)

enable_testing()

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_Declare(
    catch
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(json catch)

add_executable(grocery-shopping-list
    src/app/App.cpp
    src/ui/MainFrame.cpp
)
target_compile_options(grocery-shopping-list PRIVATE -Wall -Wextra)
target_include_directories(grocery-shopping-list PRIVATE src)
target_link_libraries(grocery-shopping-list PRIVATE
    ${wxWidgets_LIBRARIES}
    nlohmann_json::nlohmann_json
)

add_executable(grocery-shopping-list-tests
    tests/domain/Placeholder_test.cpp
)
target_compile_options(grocery-shopping-list-tests PRIVATE -Wall -Wextra)
target_include_directories(grocery-shopping-list-tests PRIVATE src)
target_link_libraries(grocery-shopping-list-tests PRIVATE
    Catch2::Catch2WithMain
    nlohmann_json::nlohmann_json
)

include(CTest)
include(Catch)
catch_discover_tests(grocery-shopping-list-tests)
