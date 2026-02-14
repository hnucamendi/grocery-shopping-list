# Story 1.1: Project Setup & Application Window

Status: ready-for-dev

## Story

As a developer/user,
I want to launch a macOS desktop application with a clean empty window,
So that I have a working application foundation to build grocery list features on.

## Acceptance Criteria

1. **Given** the project is cloned and dependencies are installed (brew install wxwidgets cmake)
   **When** the developer runs `cmake -B build && cmake --build build`
   **Then** the project compiles with zero warnings on `-Wall -Wextra`
   **And** a single executable is produced

2. **Given** the application executable exists
   **When** the user launches the application
   **Then** a standalone macOS desktop window appears with title "Grocery Shopping List"
   **And** the window has a minimum size of 600x400
   **And** the window uses native macOS controls and styling

3. **Given** the project is built
   **When** the developer inspects the project structure
   **Then** the directory structure matches the architecture spec: src/app/, src/domain/, src/persistence/, src/ui/, tests/domain/, tests/persistence/, data/lists/, data/images/
   **And** CMakeLists.txt uses find_package(wxWidgets) and FetchContent for nlohmann/json and Catch2
   **And** C++17 standard is configured

4. **Given** the test target exists
   **When** the developer runs the test executable
   **Then** Catch2 test framework runs successfully (even if only a placeholder test exists)

## Tasks / Subtasks

- [ ] Task 1: Create project directory structure (AC: #3)
  - [ ] Create src/app/, src/domain/, src/persistence/, src/ui/
  - [ ] Create tests/domain/, tests/persistence/
  - [ ] Create data/lists/, data/images/
  - [ ] Create placeholder.png in data/images/ (or a simple placeholder file)
  - [ ] Create .gitignore (exclude build/, data/lists/*.json)
- [ ] Task 2: Write root CMakeLists.txt (AC: #1, #3)
  - [ ] Set cmake_minimum_required(VERSION 3.16)
  - [ ] Set C++17 standard with CMAKE_CXX_STANDARD 17
  - [ ] Add -Wall -Wextra compiler flags
  - [ ] Configure find_package(wxWidgets REQUIRED COMPONENTS core base)
  - [ ] Configure FetchContent for nlohmann/json 3.12.0 and Catch2 v3.9.1
  - [ ] Define main executable target
  - [ ] Define test executable target
- [ ] Task 3: Create App entry point (AC: #2)
  - [ ] Create src/app/App.h — wxApp subclass declaration
  - [ ] Create src/app/App.cpp — OnInit() implementation
  - [ ] wxIMPLEMENT_APP(App) macro in App.cpp
- [ ] Task 4: Create MainFrame (AC: #2)
  - [ ] Create src/ui/MainFrame.h — wxFrame subclass declaration
  - [ ] Create src/ui/MainFrame.cpp — constructor with title "Grocery Shopping List" and min size 600x400
  - [ ] SetMinSize(wxSize(600, 400)) in constructor
- [ ] Task 5: Create placeholder test (AC: #4)
  - [ ] Create tests/domain/Placeholder_test.cpp with a single Catch2 TEST_CASE
  - [ ] Verify test target compiles and runs
- [ ] Task 6: Build verification (AC: #1)
  - [ ] Run cmake -B build -DCMAKE_BUILD_TYPE=Debug
  - [ ] Run cmake --build build
  - [ ] Verify zero warnings on -Wall -Wextra
  - [ ] Launch executable and verify window appears
  - [ ] Run test executable and verify Catch2 passes

## Dev Notes

### Architecture Compliance

**Layer Structure (strictly enforced):**
- Domain -> nothing (zero dependencies on persistence or UI)
- Persistence -> Domain only (no wxWidgets imports)
- UI -> Domain only (no persistence imports directly)
- App -> all layers (wires everything together)

For this story, only App and UI layers are active. Domain and Persistence directories are created but empty (except placeholder test).

### Naming Conventions

- **Classes:** PascalCase — `App`, `MainFrame`
- **Member functions:** camelCase
- **Member variables:** m_ prefix + camelCase
- **Source files:** PascalCase.cpp / PascalCase.h (file name matches class name)
- **Test files:** PascalCase_test.cpp
- **Header guards:** PROJECT_DIRECTORY_FILENAME_H pattern

### Technical Requirements

**C++17 Standard:**
- Set CMAKE_CXX_STANDARD to 17
- Set CMAKE_CXX_STANDARD_REQUIRED ON
- Set CMAKE_CXX_EXTENSIONS OFF (use -std=c++17, not -std=gnu++17)

**Compiler Flags (NFR6):**
- -Wall -Wextra must produce zero warnings
- Configure via target_compile_options or CMAKE_CXX_FLAGS

**Memory Management (NFR7):**
- No raw new/delete
- Use RAII, smart pointers, stack allocation
- wxWidgets manages its own widget lifecycle (parent-child ownership)

### Library & Framework Requirements

**wxWidgets 3.2.9 (via Homebrew):**
- Install: `brew install wxwidgets`
- CMake: `find_package(wxWidgets REQUIRED COMPONENTS core base)`
- Must include `${wxWidgets_USE_FILE}` after find_package
- Link with `${wxWidgets_LIBRARIES}`
- On Apple Silicon: wx-config at `/opt/homebrew/bin/wx-config`
- On Intel: wx-config at `/usr/local/opt/wxwidgets/bin/wx-config`

**nlohmann/json 3.12.0 (via FetchContent):**
```cmake
FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_MakeAvailable(json)
```
- Link target: `nlohmann_json::nlohmann_json`
- Not needed for this story but must be configured in CMakeLists.txt

**Catch2 v3.9.1 (via FetchContent):**
```cmake
FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.9.1
)
FetchContent_MakeAvailable(Catch2)
```
- CRITICAL: This is Catch2 v3, NOT v2
- Include pattern: `#include <catch2/catch_all.hpp>` (NOT `#include <catch.hpp>`)
- Link target: `Catch2::Catch2WithMain` (no custom main needed)
- Test run order is random by default in v3.9.x

### File Structure Requirements

**Complete directory structure to create:**
```
grocery-shopping-list/
├── CMakeLists.txt
├── .gitignore
├── src/
│   ├── app/
│   │   ├── App.h
│   │   └── App.cpp
│   ├── domain/          (empty for now)
│   ├── persistence/     (empty for now)
│   └── ui/
│       ├── MainFrame.h
│       └── MainFrame.cpp
├── tests/
│   ├── domain/
│   │   └── Placeholder_test.cpp
│   └── persistence/     (empty for now)
├── data/
│   ├── lists/
│   └── images/
│       └── placeholder.png
└── build/               (gitignored)
```

### Testing Requirements

- Catch2 v3 test framework must compile and run
- Placeholder test validates the test infrastructure works
- No domain or persistence tests yet (those layers are empty)
- Test executable should be a separate CMake target

### CMakeLists.txt Reference Structure

```cmake
cmake_minimum_required(VERSION 3.16)
project(grocery-shopping-list LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# FetchContent dependencies
include(FetchContent)

FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.9.1
)

FetchContent_MakeAvailable(json Catch2)

# Main executable
add_executable(grocery-shopping-list
    src/app/App.cpp
    src/ui/MainFrame.cpp
)
target_compile_options(grocery-shopping-list PRIVATE -Wall -Wextra)
target_link_libraries(grocery-shopping-list PRIVATE ${wxWidgets_LIBRARIES})

# Test executable
add_executable(grocery-tests
    tests/domain/Placeholder_test.cpp
)
target_compile_options(grocery-tests PRIVATE -Wall -Wextra)
target_link_libraries(grocery-tests PRIVATE Catch2::Catch2WithMain)

enable_testing()
add_test(NAME grocery-tests COMMAND grocery-tests)
```

### Build & Run Commands

```bash
# Install dependencies
brew install wxwidgets cmake

# Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Run app
./build/grocery-shopping-list

# Run tests
./build/grocery-tests
```

### Project Structure Notes

- This is a greenfield project — no existing code to preserve
- The directory structure follows the architecture document exactly
- Empty directories (domain/, persistence/) establish the layered architecture from day one
- The .gitignore should exclude: build/, data/lists/*.json, .DS_Store

### Anti-Patterns to Reject

- Do NOT use snake_case for C++ function names (use camelCase)
- Do NOT use raw new/delete (use RAII, smart pointers)
- Do NOT use Catch2 v2 include pattern (`#include <catch.hpp>`)
- Do NOT put all source files in a flat directory
- Do NOT skip -Wall -Wextra flags

### References

- [Source: _bmad-output/planning-artifacts/architecture.md#Starter Template Evaluation]
- [Source: _bmad-output/planning-artifacts/architecture.md#Project Structure & Boundaries]
- [Source: _bmad-output/planning-artifacts/architecture.md#Implementation Patterns & Consistency Rules]
- [Source: _bmad-output/planning-artifacts/epics.md#Story 1.1]
- [Source: _bmad-output/planning-artifacts/prd.md#Technical Architecture]
- [Source: _bmad-output/planning-artifacts/ux-design-specification.md#Implementation Approach]

## Dev Agent Record

### Agent Model Used

### Debug Log References

### Completion Notes List

### File List
