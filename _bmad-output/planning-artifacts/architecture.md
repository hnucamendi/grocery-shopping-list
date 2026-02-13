---
stepsCompleted: [1, 2, 3, 4, 5, 6, 7, 8]
lastStep: 8
status: 'complete'
completedAt: '2026-02-13'
inputDocuments: ['_bmad-output/planning-artifacts/prd.md', '_bmad-output/planning-artifacts/ux-design-specification.md']
workflowType: 'architecture'
project_name: 'grocery-shopping-list'
user_name: 'Hnucamend'
date: '2026-02-13'
---

# Architecture Decision Document

_This document builds collaboratively through step-by-step discovery. Sections are appended as we work through each architectural decision together._

## Project Context Analysis

### Requirements Overview

**Functional Requirements:**
20 functional requirements across 5 categories:
- **List Management (FR1-6):** CRUD operations on grocery items — add, edit, delete items with name, quantity, and store section
- **Item Organization (FR7-9):** Display items grouped by store section with predefined section options
- **Data Persistence (FR10-12):** Auto-save to local file, auto-load on startup, full data fidelity across restarts
- **List History (FR13-16):** Browse past lists by date, distinguish current vs historical lists
- **User Interface (FR17-20):** macOS desktop window, text input interface, scrollable grouped list view

Architecturally, this is a straightforward CRUD + persistence pattern with a visual grouping layer.

**Non-Functional Requirements:**
- **NFR1-2 (Data Integrity):** Graceful shutdown handling, no data corruption — drives file I/O strategy (atomic writes, safe serialization)
- **NFR3-5 (Usability):** Clean minimal design, max 3 interactions to add an item, scannable section grouping — satisfied by UX spec's wxWidgets layout
- **NFR6-7 (Code Quality):** Zero compiler warnings on `-Wall -Wextra`, RAII patterns, no raw memory management — compile-time and design-time discipline
- **NFR8 (Extensibility):** Architecture must support adding Recipes and Budget epics without rewriting existing modules — the primary architectural driver

**Scale & Complexity:**

- Primary domain: Desktop GUI application (C++ / wxWidgets / macOS)
- Complexity level: Low
- Estimated architectural components: 4-6 (UI layer, data model, persistence, image handling, section management, list history management)

### Technical Constraints & Dependencies

- **Language:** C++ (intermediate skill level, learning-focused)
- **GUI Framework:** wxWidgets (confirmed by UX spec)
- **Platform:** macOS only, no cross-platform requirement
- **Storage:** Local filesystem only (JSON or similar), no database
- **Build System:** Makefile or CMake
- **Images:** Local filesystem, user-provided with generic fallback
- **No external service dependencies**

### Cross-Cutting Concerns Identified

- **Auto-save persistence:** Every add, edit, and delete triggers a save — persistence is woven through all write paths
- **Extensibility boundaries:** Module interfaces must be designed so Recipes and Budget features plug in without rewriting core list management
- **Image loading:** Item cards display images from local filesystem — image I/O intersects with both the data model (image path per item) and UI rendering
- **Section grouping:** The grouping logic affects both the data model (items have sections) and UI layout (section headers + card grids) — a concern shared across layers

## Starter Template Evaluation

### Primary Technology Domain

Desktop GUI application — C++ with wxWidgets on macOS. This is a native desktop app with no web, mobile, or cloud components.

### Starter Options Considered

**Option 1: lszl84/wx_cmake_template**
CMake superbuild that auto-downloads wxWidgets. Good for zero-config setup but adds CMake complexity with nested ExternalProject_Add patterns. Overkill for a single-platform learning project.

**Option 2: Ravbug/wxWidgetsTemplate**
Pre-configured IDE projects with wxWidgets as git submodule. Includes Xcode/VS/Make files. Too heavy — most IDE configs unused for a macOS-only project.

**Option 3: Manual CMake + Homebrew wxWidgets (Selected)**
Minimal hand-written CMakeLists.txt using find_package(wxWidgets) with wxWidgets installed via Homebrew. Maximum learning value, minimal abstraction.

### Selected Approach: Manual CMake + Homebrew wxWidgets

**Rationale for Selection:**
- Learning-first project benefits from understanding the build system
- CMakeLists.txt is minimal (~15 lines) and fully comprehensible
- Homebrew handles wxWidgets installation cleanly on macOS
- CMake proficiency is a transferable C++ skill
- No template abstractions hiding build mechanics

**Initialization Commands:**

```bash
brew install wxwidgets cmake
mkdir -p src include
# CMakeLists.txt written manually as first implementation task
```

**Architectural Decisions Established:**

**Language & Runtime:**
- C++17 standard (modern features, wide compiler support)
- Apple Clang (Xcode Command Line Tools)
- Compiler flags: -Wall -Wextra (per NFR6)

**GUI Framework:**
- wxWidgets 3.2.9 (latest stable via Homebrew)
- Native macOS controls and styling

**Data Serialization:**
- nlohmann/json 3.12.0 (header-only, via CMake FetchContent)
- Local JSON file storage for list persistence

**Build Tooling:**
- CMake (minimum 3.16+)
- find_package(wxWidgets REQUIRED COMPONENTS core base) for wxWidgets linking
- FetchContent for nlohmann/json

**Testing Framework:**
- TBD in architecture decisions (lightweight, if any — learning project)

**Code Organization:**
- src/ for implementation files
- include/ for headers
- data/ for local storage (JSON files, images)
- Flat structure initially, modularize as complexity grows

**Development Experience:**
- cmake --build for compilation
- Zero warnings enforcement via compiler flags
- No IDE requirement — works with any editor + terminal

## Core Architectural Decisions

### Decision Priority Analysis

**Critical Decisions (Block Implementation):**
- Data storage format: One JSON file per list
- Application layering: UI → Domain → Persistence
- Auto-save strategy: Save on every mutation

**Important Decisions (Shape Architecture):**
- Observer pattern for UI-domain communication
- Image caching with filesystem fallback
- Catch2 for domain/persistence testing

**Deferred Decisions (Post-MVP):**
- Recipe data model structure (Epic 1)
- Budget/price data model structure (Epic 4)
- Search/filter architecture (Phase 3)

### Data Architecture

- **Storage format:** One JSON file per grocery list, named by date (e.g., `2026-02-13.json`)
- **Storage location:** `data/lists/` directory within app's working directory
- **Serialization:** nlohmann/json 3.12.0 — direct struct-to-JSON mapping
- **Validation:** Validate on load only — trust in-app writes, guard against corruption or manual edits
- **Auto-save:** Save on every mutation (add, edit, delete) — file sizes are trivial (20-50 items)
- **Data integrity (NFR1-2):** Atomic write pattern — write to temp file, then rename. Prevents corruption on crash mid-write.
- **Rationale:** Per-file storage maps directly to list history browsing, isolates corruption risk, and simplifies the persistence layer

### Authentication & Security

Not applicable. Single user, local machine, no network, no accounts.

### API & Communication Patterns

Not applicable. Pure local desktop application with no network communication.

### Application Architecture

**Layered Architecture (3 layers):**
- **UI Layer:** wxWidgets frames, panels, sizers, event handlers. Responsible for display and user input. Depends on Domain layer.
- **Domain Layer:** Core data models (GroceryItem, GroceryList, StoreSection). Business logic, validation, grouping. No wxWidgets dependencies.
- **Persistence Layer:** JSON serialization, file I/O, directory management. No wxWidgets dependencies.

**Communication Pattern:** Observer pattern
- Domain objects notify registered listeners on data changes
- UI panels register as observers to refresh on updates
- Supports NFR8 extensibility — new UI panels observe existing data without modifying it
- Classic C++ design pattern exercise

**Image Management:**
- GroceryItem model stores optional `imagePath` string
- In-memory wxBitmap cache (std::unordered_map<string, wxBitmap>) avoids redundant disk reads
- Fallback placeholder image for items without a custom image
- Images loaded/scaled on first access, cached for subsequent renders

### Infrastructure & Deployment

**Build Configuration:**
- Single executable target — no separate libraries
- CMake Debug/Release via CMAKE_BUILD_TYPE
- Debug: symbols enabled, no optimization
- Release: optimization enabled, no debug symbols

**Testing:**
- Catch2 (header-only, via CMake FetchContent) for unit tests
- Test domain layer: data models, grouping logic, validation
- Test persistence layer: JSON serialization, file I/O, atomic writes
- No UI testing — manual verification for wxWidgets components

**Logging & Error Handling:**
- wxLog (built-in) for diagnostic messages
- Exceptions for truly exceptional cases: file I/O failures, corrupt data on load
- std::optional for expected empty/missing results (item not found, no image path)
- No raw error codes — modern C++ idioms throughout

### Decision Impact Analysis

**Implementation Sequence:**
1. CMake project setup + wxWidgets linking + Catch2 integration
2. Domain layer: GroceryItem, GroceryList, StoreSection models
3. Persistence layer: JSON serialization + atomic file writes
4. UI layer: main frame, input bar, section-grouped card grid
5. Integration: wire Observer pattern between domain and UI
6. History: directory-based list discovery and read-only display

**Cross-Component Dependencies:**
- Domain layer is independent — no dependencies on UI or persistence
- Persistence depends on Domain (serializes domain objects)
- UI depends on Domain (displays and mutates domain objects)
- Observer pattern couples UI to Domain via abstract interface, not concrete classes
- Image cache lives in UI layer, image paths live in Domain layer

## Implementation Patterns & Consistency Rules

### Pattern Categories Defined

**Critical Conflict Points Identified:**
5 areas where AI agents could make different choices: naming conventions, project structure, JSON format, observer communication, and error handling flow.

### Naming Patterns

**C++ Code Naming Conventions:**
- **Classes/Structs:** `PascalCase` — `GroceryItem`, `GroceryList`, `StoreSection`
- **Member functions:** `camelCase` — `addItem()`, `getItemsBySection()`, `removeItem()`
- **Member variables:** `m_` prefix + `camelCase` — `m_name`, `m_quantity`, `m_section`
- **Local variables:** `camelCase` — `itemCount`, `sectionName`
- **Constants/Enums:** `PascalCase` for enum type, `PascalCase` for values — `enum class StoreSection { Produce, Dairy, Meat, Frozen, Bakery, Pantry, Household }`
- **Header guards:** `PROJECT_DIRECTORY_FILENAME_H` — `GROCERY_INCLUDE_GROCERYITEM_H`
- **Namespaces:** lowercase — `grocery`, `grocery::persistence`

**File Naming Conventions:**
- **Source files:** `PascalCase.cpp` — `GroceryItem.cpp`, `ListStorage.cpp`
- **Header files:** `PascalCase.h` — `GroceryItem.h`, `ListStorage.h`
- **Test files:** `PascalCase_test.cpp` — `GroceryItem_test.cpp`

**JSON Field Naming:**
- `snake_case` in JSON files — `store_section`, `item_name`, `created_date`
- Maps to C++ member variables via explicit nlohmann/json serialization

### Structure Patterns

**Project Organization:**
- `src/domain/` — GroceryItem, GroceryList, StoreSection, Observer interface
- `src/persistence/` — ListStorage, JsonSerializer
- `src/ui/` — MainFrame, ItemCard, SectionGroup, InputBar, ImageCache
- `src/app/` — App entry point (wxApp subclass)
- `tests/domain/` — GroceryItem_test.cpp, GroceryList_test.cpp
- `tests/persistence/` — ListStorage_test.cpp, JsonSerializer_test.cpp
- `data/lists/` — JSON list files
- `data/images/` — user-provided item images
- `data/images/placeholder.png` — fallback image

**File Structure Rules:**
- Every `.h` has a corresponding `.cpp` (no header-only classes except pure interfaces)
- One class per file — file name matches class name exactly
- Headers and sources co-located within each layer directory

### Format Patterns

**JSON List File Structure:**
```json
{
  "created_date": "2026-02-13",
  "items": [
    {
      "name": "Chicken Breast",
      "quantity": "2 lbs",
      "store_section": "meat",
      "image_path": "chicken.png"
    }
  ]
}
```

**JSON Format Rules:**
- `store_section` values: lowercase string matching enum — `produce`, `dairy`, `meat`, `frozen`, `bakery`, `pantry`, `household`
- `image_path`: relative to `data/images/`, empty string or omitted if no image
- `quantity`: free-text string (not numeric) — user types "2 lbs", "1 bunch", "3"
- File naming: `YYYY-MM-DD.json` (e.g., `2026-02-13.json`)

### Communication Patterns

**Observer Interface:**
- Single abstract interface: `IListObserver` with `onListChanged()` method
- Domain layer defines the interface, UI layer implements it
- No event payload — observers re-read the full list state on notification
- Registration via `addObserver()` / `removeObserver()` on `GroceryList`

**State Update Rules:**
- All mutations go through `GroceryList` methods — never modify `GroceryItem` directly from UI
- `GroceryList` notifies observers after every mutation
- `GroceryList` triggers persistence save after every mutation

### Process Patterns

**Error Handling:**
- File I/O errors: throw `std::runtime_error` with descriptive message — caught at UI layer, shown via `wxLogError`
- JSON parse errors on load: log warning via `wxLogWarning`, skip corrupt file, continue loading other lists
- Missing image file: silently use placeholder — no error, no log
- Empty/missing data directory: create it automatically on first save

**Loading Flow:**
- App startup: scan `data/lists/` directory, load most recent list by date, display in grid
- If no lists exist: show empty grid with input bar ready
- If current list file is corrupt: log warning, show empty grid, don't delete the corrupt file

### Enforcement Guidelines

**All AI Agents MUST:**
- Follow naming conventions exactly — no exceptions, no "personal style" overrides
- Place files in the correct layer directory — domain code never imports from ui or persistence
- Use the JSON format structure exactly as defined — field names, casing, and types must match
- Route all data mutations through GroceryList — no direct item modification from UI code
- Use std::optional for missing values, exceptions for I/O failures — never error codes

**Anti-Patterns to Reject:**
- `snake_case` function names in C++ code (use `camelCase`)
- Raw `new`/`delete` anywhere (use smart pointers, RAII, stack allocation)
- Direct file writes without atomic temp-file-then-rename pattern
- UI code that directly reads/writes JSON files (must go through persistence layer)
- Observer callbacks that modify data (observers are read-only responders)

## Project Structure & Boundaries

### Complete Project Directory Structure

```
grocery-shopping-list/
├── CMakeLists.txt
├── .gitignore
├── src/
│   ├── CMakeLists.txt
│   ├── app/
│   │   ├── App.h
│   │   └── App.cpp
│   ├── domain/
│   │   ├── GroceryItem.h
│   │   ├── GroceryItem.cpp
│   │   ├── GroceryList.h
│   │   ├── GroceryList.cpp
│   │   ├── StoreSection.h
│   │   ├── StoreSection.cpp
│   │   ├── IListObserver.h
│   │   ├── ListManager.h
│   │   └── ListManager.cpp
│   ├── persistence/
│   │   ├── JsonSerializer.h
│   │   ├── JsonSerializer.cpp
│   │   ├── ListStorage.h
│   │   └── ListStorage.cpp
│   └── ui/
│       ├── MainFrame.h
│       ├── MainFrame.cpp
│       ├── InputBar.h
│       ├── InputBar.cpp
│       ├── ItemCard.h
│       ├── ItemCard.cpp
│       ├── SectionGroup.h
│       ├── SectionGroup.cpp
│       ├── ItemGridPanel.h
│       ├── ItemGridPanel.cpp
│       ├── ImageCache.h
│       └── ImageCache.cpp
├── tests/
│   ├── CMakeLists.txt
│   ├── domain/
│   │   ├── GroceryItem_test.cpp
│   │   ├── GroceryList_test.cpp
│   │   ├── StoreSection_test.cpp
│   │   └── ListManager_test.cpp
│   └── persistence/
│       ├── JsonSerializer_test.cpp
│       └── ListStorage_test.cpp
├── data/
│   ├── lists/
│   └── images/
│       └── placeholder.png
└── build/
```

### Architectural Boundaries

**Layer Dependencies (strictly enforced):**
- **Domain → nothing.** Domain classes have zero dependencies on persistence or UI. They include only STL headers and IListObserver.h.
- **Persistence → Domain.** JsonSerializer and ListStorage include domain headers to serialize/deserialize. They never include wxWidgets headers.
- **UI → Domain.** UI classes include domain headers and wxWidgets headers. They never include persistence headers directly.
- **App → all layers.** App.cpp wires everything together: creates ListStorage, creates GroceryList, creates MainFrame, registers observers.

### Requirements to Structure Mapping

**Feature Mapping:**

| FR Category | Layer | Files |
|---|---|---|
| List Management (FR1-6) | Domain | GroceryItem.h/.cpp, GroceryList.h/.cpp |
| Item Organization (FR7-9) | Domain + UI | StoreSection.h/.cpp, SectionGroup.h/.cpp |
| Data Persistence (FR10-12) | Persistence | ListStorage.h/.cpp, JsonSerializer.h/.cpp |
| List History (FR13-16) | Persistence + UI | ListManager.h/.cpp, MainFrame.h/.cpp |
| User Interface (FR17-20) | UI | MainFrame, InputBar, ItemCard, ItemGridPanel |

**Cross-Cutting Concerns:**
- Auto-save: GroceryList mutation → App wiring triggers ListStorage::save()
- Image handling: ImageCache (UI layer) loads from data/images/, domain stores path only
- Section grouping: StoreSection enum (domain) drives SectionGroup layout (UI)

### Integration Points

**Internal Communication:**
- UI ↔ Domain: Observer pattern (IListObserver::onListChanged())
- Domain ↔ Persistence: App wiring connects them — GroceryList doesn't know about ListStorage
- ListManager coordinates active list and delegates to ListStorage for loading/saving

**Data Flow:**
```
User Input → InputBar → MainFrame → GroceryList::addItem()
                                         ↓
                                   GroceryList notifies observers
                                         ↓                    ↓
                              ItemGridPanel refreshes    ListStorage saves to JSON
```

**Startup Flow:**
1. App::OnInit creates ListStorage with data/lists/ path
2. ListStorage scans directory, returns available dates
3. App creates ListManager, loads most recent list via ListStorage → JsonSerializer → GroceryList
4. App creates MainFrame, passes GroceryList reference
5. MainFrame registers as observer on GroceryList

### Development Workflow

**Build:**
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/grocery-shopping-list
```

**Test:**
```bash
cmake --build build --target tests
./build/tests/grocery-tests
```

## Architecture Validation Results

### Coherence Validation ✅

**Decision Compatibility:**
All technology choices are compatible: C++17 + wxWidgets 3.2.9 + nlohmann/json 3.12.0 + Catch2 all support C++17, work with CMake, and compile under Apple Clang. Homebrew wxWidgets via find_package and FetchContent for json/catch2 is a clean dependency strategy with no conflicts.

**Pattern Consistency:**
Naming conventions (PascalCase classes, camelCase functions, m_ members, snake_case JSON) are internally consistent with no contradictions. Observer pattern aligns cleanly with the layered architecture.

**Structure Alignment:**
Directory structure maps directly to the three-layer architecture. Each layer has its own src/ subdirectory. Tests mirror source structure. No orphan files or ambiguous placements.

### Requirements Coverage Validation ✅

**Functional Requirements Coverage:**
- FR1-6 (List CRUD): GroceryList + GroceryItem in domain layer ✅
- FR7-9 (Section grouping): StoreSection enum + SectionGroup UI component ✅
- FR10-12 (Persistence): ListStorage + JsonSerializer + atomic writes ✅
- FR13-16 (History): ListManager + per-file storage + MainFrame selector ✅
- FR17-20 (UI): MainFrame + InputBar + ItemCard + ItemGridPanel ✅

**Non-Functional Requirements Coverage:**
- NFR1-2 (Data integrity): Atomic write pattern (temp file + rename) ✅
- NFR3-5 (Usability): wxWidgets native controls, 3-field input bar ✅
- NFR6 (Zero warnings): -Wall -Wextra compiler flags in CMake ✅
- NFR7 (No raw memory): RAII mandate, anti-pattern enforcement ✅
- NFR8 (Extensibility): Layered architecture + Observer pattern + module boundaries ✅

### Implementation Readiness Validation ✅

**Decision Completeness:** All critical decisions documented with specific technology versions and rationale.

**Structure Completeness:** Every file and directory specified with clear purpose and layer assignment.

**Pattern Completeness:** Naming, structure, format, communication, and process patterns all defined with concrete examples and anti-patterns to reject.

### Gap Analysis Results

**Critical Gaps:** None.

**Important Gaps (non-blocking):**
1. **Multiple lists per day:** YYYY-MM-DD.json naming assumes one list per day. If two lists are created on the same date, the second overwrites the first. Acceptable for MVP (weekly list building pattern). Future enhancement: append counter or use UUID-based naming.
2. **ListManager wiring:** Conceptually described but less precisely specified than other components. App.cpp wiring will require careful implementation attention.

**Nice-to-Have Gaps:**
- No .clang-format configuration — agents may format code differently. Low risk for solo project.

### Architecture Completeness Checklist

**✅ Requirements Analysis**
- [x] Project context thoroughly analyzed
- [x] Scale and complexity assessed
- [x] Technical constraints identified
- [x] Cross-cutting concerns mapped

**✅ Architectural Decisions**
- [x] Critical decisions documented with versions
- [x] Technology stack fully specified
- [x] Integration patterns defined
- [x] Performance considerations addressed

**✅ Implementation Patterns**
- [x] Naming conventions established
- [x] Structure patterns defined
- [x] Communication patterns specified
- [x] Process patterns documented

**✅ Project Structure**
- [x] Complete directory structure defined
- [x] Component boundaries established
- [x] Integration points mapped
- [x] Requirements to structure mapping complete

### Architecture Readiness Assessment

**Overall Status:** READY FOR IMPLEMENTATION

**Confidence Level:** High

**Key Strengths:**
- Clean layered architecture with strictly enforced boundaries
- Every requirement has explicit architectural support
- Implementation patterns are specific enough to prevent agent conflicts
- Technology choices are simple, well-understood, and compatible
- Extensibility built in from the start via Observer pattern and module boundaries

**Areas for Future Enhancement:**
- Multiple-lists-per-day naming strategy (post-MVP)
- .clang-format for automated code formatting consistency
- ListManager interface formalization if complexity grows

### Implementation Handoff

**AI Agent Guidelines:**
- Follow all architectural decisions exactly as documented
- Use implementation patterns consistently across all components
- Respect project structure and layer boundaries
- Refer to this document for all architectural questions

**First Implementation Priority:**
1. Install dependencies: `brew install wxwidgets cmake`
2. Create CMakeLists.txt with find_package(wxWidgets) + FetchContent for nlohmann/json and Catch2
3. Create project directory structure (src/app, src/domain, src/persistence, src/ui, tests/, data/)
4. Build minimal App.cpp + MainFrame.cpp to verify wxWidgets compilation
