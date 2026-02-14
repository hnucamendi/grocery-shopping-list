# Story 2.1: JSON Serialization & Auto-Save

Status: ready-for-dev

## Story

As a grocery shopper,
I want my grocery list to be automatically saved every time I add an item,
So that I never lose my list if I close the app or it shuts down unexpectedly.

## Acceptance Criteria

1. **Given** the user adds an item to the current list
   **When** the item is created in GroceryList
   **Then** the entire list is automatically saved to a JSON file in data/lists/
   **And** the file is named by date (e.g., 2026-02-13.json)
   **And** no manual save action is required from the user

2. **Given** a save is triggered
   **When** the system writes the list to disk
   **Then** the atomic write pattern is used: write to a temp file first, then rename to the final filename
   **And** an interrupted write does not corrupt the existing saved file

3. **Given** a list is saved to JSON
   **When** inspecting the file contents
   **Then** the JSON structure matches: `created_date`, `items` array with `name`, `quantity`, `store_section` fields
   **And** JSON field names use snake_case
   **And** store_section values are lowercase strings matching the enum

4. **Given** the data/lists/ directory does not exist
   **When** the first save is triggered
   **Then** the directory is created automatically

5. **Given** the persistence layer is implemented
   **When** inspecting the code
   **Then** JsonSerializer handles struct-to-JSON mapping using nlohmann/json
   **And** ListStorage handles file I/O with atomic writes
   **And** persistence layer depends on domain layer only — no wxWidgets imports
   **And** unit tests exist for JsonSerializer and ListStorage

## Tasks / Subtasks

- [ ] Task 1: Create JsonSerializer (AC: #3, #5)
  - [ ] Create src/persistence/JsonSerializer.h and JsonSerializer.cpp
  - [ ] Implement toJson(const GroceryList&) -> nlohmann::json
  - [ ] Implement fromJson(const nlohmann::json&) -> GroceryList
  - [ ] Map StoreSection enum to/from lowercase strings
  - [ ] Handle quantity as free-text string
  - [ ] Include created_date field (YYYY-MM-DD format)
- [ ] Task 2: Create ListStorage (AC: #1, #2, #4, #5)
  - [ ] Create src/persistence/ListStorage.h and ListStorage.cpp
  - [ ] Constructor takes data directory path (e.g., "data/lists/")
  - [ ] Implement save(const GroceryList&) using atomic write pattern
  - [ ] Atomic write: write to temp file, then std::filesystem::rename to final
  - [ ] Auto-create data/lists/ directory if missing (std::filesystem::create_directories)
  - [ ] File naming: YYYY-MM-DD.json based on list's created_date
- [ ] Task 3: Wire auto-save into App (AC: #1)
  - [ ] In App.cpp, after GroceryList mutation (addItem), call ListStorage::save()
  - [ ] GroceryList notifies observers -> App layer triggers persistence
  - [ ] Persistence wiring lives in App layer, NOT in domain or UI
- [ ] Task 4: Write unit tests (AC: #5)
  - [ ] Create tests/persistence/JsonSerializer_test.cpp
  - [ ] Test round-trip: GroceryList -> JSON -> GroceryList (all fields preserved)
  - [ ] Test store_section serialization (enum -> lowercase string -> enum)
  - [ ] Test empty list serialization
  - [ ] Create tests/persistence/ListStorage_test.cpp
  - [ ] Test save creates file at expected path
  - [ ] Test atomic write (temp file pattern)
  - [ ] Test directory auto-creation
- [ ] Task 5: Build verification
  - [ ] Verify zero warnings with -Wall -Wextra
  - [ ] Run all tests (domain + persistence)
  - [ ] Add item in app, verify JSON file appears in data/lists/

## Dev Notes

### Architecture Compliance

**Layer boundaries (strictly enforced):**
- Persistence layer depends on Domain ONLY — no wxWidgets imports
- JsonSerializer and ListStorage include domain headers only
- Auto-save wiring happens in App layer (App.cpp), NOT in domain or UI
- UI never imports from persistence directly

**Data flow for auto-save:**
```
User adds item -> InputBar -> MainFrame -> GroceryList::addItem()
                                                ↓
                                    GroceryList notifies observers
                                         ↓                ↓
                              ItemGridPanel refreshes   App triggers ListStorage::save()
```

### Naming Conventions

- **Classes:** PascalCase — `JsonSerializer`, `ListStorage`
- **Member functions:** camelCase — `toJson()`, `fromJson()`, `save()`
- **Member variables:** m_ prefix — `m_dataDir`
- **Files:** PascalCase.cpp/.h — `JsonSerializer.cpp`, `ListStorage.cpp`
- **Test files:** PascalCase_test.cpp
- **JSON fields:** snake_case — `created_date`, `store_section`, `image_path`

### Technical Requirements

**nlohmann/json 3.12.0 usage:**
```cpp
#include <nlohmann/json.hpp>
using json = nlohmann::json;
```
- Use explicit to_json/from_json functions for GroceryItem and GroceryList
- Do NOT use NLOHMANN_DEFINE_TYPE macros (explicit mapping is clearer for learning)

**Atomic write pattern (NFR1-2):**
```cpp
// 1. Write to temp file
std::string tempPath = finalPath + ".tmp";
std::ofstream out(tempPath);
out << jsonData.dump(2);
out.close();
// 2. Rename (atomic on POSIX)
std::filesystem::rename(tempPath, finalPath);
```

**JSON file format (exact):**
```json
{
  "created_date": "2026-02-13",
  "items": [
    {
      "name": "Chicken Breast",
      "quantity": "2 lbs",
      "store_section": "meat"
    }
  ]
}
```

**StoreSection enum -> string mapping:**
- Produce -> "produce", Dairy -> "dairy", Meat -> "meat", Frozen -> "frozen", Bakery -> "bakery", Pantry -> "pantry", Household -> "household"

### File Structure

```
src/persistence/
├── JsonSerializer.h
├── JsonSerializer.cpp
├── ListStorage.h
└── ListStorage.cpp

tests/persistence/
├── JsonSerializer_test.cpp
└── ListStorage_test.cpp

data/lists/           (auto-created, gitignored)
```

### Dependencies on Previous Stories

- **Story 1.1:** Project structure, CMakeLists.txt, App.h/.cpp, MainFrame
- **Story 1.2:** GroceryItem, GroceryList, StoreSection (domain layer must exist)
- **Story 1.3:** Observer pattern, ItemGridPanel (UI must exist for wiring context)

### Anti-Patterns to Reject

- Do NOT save from within GroceryList (domain must not know about persistence)
- Do NOT save from UI layer (UI must not import persistence)
- Do NOT use raw file writes without atomic pattern
- Do NOT use numeric store_section values in JSON (use lowercase strings)
- Do NOT use raw new/delete for file handling (use RAII with std::ofstream)

### References

- [Source: architecture.md#Data Architecture]
- [Source: architecture.md#Implementation Patterns - Format Patterns]
- [Source: architecture.md#Integration Points - Data Flow]
- [Source: epics.md#Story 2.1]

## Dev Agent Record

### Agent Model Used

### Debug Log References

### Completion Notes List

### File List
