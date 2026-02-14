# Story 2.2: Load List on Startup

Status: ready-for-dev

## Story

As a grocery shopper,
I want my most recent list to appear when I open the app,
So that I can pick up right where I left off without any extra steps.

## Acceptance Criteria

1. **Given** one or more saved list files exist in data/lists/
   **When** the user launches the application
   **Then** the most recent list (by date in filename) is loaded automatically
   **And** all items are displayed in the section-grouped card grid with correct names, quantities, and sections

2. **Given** no saved list files exist in data/lists/
   **When** the user launches the application
   **Then** the app displays an empty grid with the input bar ready
   **And** no error is shown

3. **Given** a saved list file is corrupt or has invalid JSON
   **When** the application attempts to load it
   **Then** a warning is logged via wxLogWarning
   **And** the corrupt file is not deleted
   **And** the app falls back to displaying an empty grid or the next most recent valid list

4. **Given** all item data was saved in a previous session
   **When** the list is loaded on startup
   **Then** every item's name, quantity, and store section is preserved exactly as it was saved (FR12)

## Tasks / Subtasks

- [ ] Task 1: Add load methods to ListStorage (AC: #1, #2, #3)
  - [ ] Implement getAvailableDates() — scan data/lists/ for *.json files, return sorted dates
  - [ ] Implement load(const std::string& date) -> GroceryList — read and deserialize
  - [ ] Implement loadMostRecent() -> std::optional<GroceryList>
  - [ ] Handle missing directory: return empty optional (no error)
  - [ ] Handle corrupt JSON: catch nlohmann::json::parse_error, log wxLogWarning, skip file
  - [ ] Do NOT delete corrupt files — preserve them
- [ ] Task 2: Add deserialization to JsonSerializer (AC: #4)
  - [ ] Implement fromJsonFile(const std::string& path) -> GroceryList
  - [ ] Validate all required fields present (name, quantity, store_section)
  - [ ] Handle missing optional fields gracefully (e.g., missing image_path -> empty string)
- [ ] Task 3: Wire startup loading into App (AC: #1, #2)
  - [ ] In App::OnInit(), after creating ListStorage, call loadMostRecent()
  - [ ] If list returned: pass to GroceryList, display in MainFrame
  - [ ] If no list: create empty GroceryList, display empty grid
  - [ ] ListManager coordinates active list and delegates to ListStorage
- [ ] Task 4: Write unit tests (AC: #3, #4)
  - [ ] Test loadMostRecent with multiple files (picks latest date)
  - [ ] Test loadMostRecent with no files (returns empty optional)
  - [ ] Test load with corrupt JSON (logs warning, returns empty)
  - [ ] Test round-trip: save then load preserves all data exactly
- [ ] Task 5: Build verification
  - [ ] Add items, close app, relaunch — items appear correctly
  - [ ] Delete all JSON files, launch — empty grid, no crash
  - [ ] Corrupt a JSON file manually, launch — warning logged, app still works

## Dev Notes

### Architecture Compliance

**Startup flow (from architecture doc):**
1. App::OnInit creates ListStorage with data/lists/ path
2. ListStorage scans directory, returns available dates
3. App creates ListManager, loads most recent list via ListStorage -> JsonSerializer -> GroceryList
4. App creates MainFrame, passes GroceryList reference
5. MainFrame registers as observer on GroceryList

**ListManager role:**
- Coordinates active list and delegates to ListStorage
- Provides the abstraction layer between App wiring and persistence
- Holds reference to current active GroceryList

### Error Handling Pattern

- File I/O errors: throw std::runtime_error — caught at App layer
- JSON parse errors on load: wxLogWarning, skip corrupt file, continue
- Missing data directory: create automatically or return empty — no error
- Use std::optional for missing/empty results (no error codes)

### File Naming Convention

- Files named YYYY-MM-DD.json (e.g., 2026-02-13.json)
- Sort by filename string to determine most recent (lexicographic date sort works)
- Use std::filesystem::directory_iterator to scan data/lists/

### Dependencies on Previous Stories

- **Story 2.1:** JsonSerializer, ListStorage (save side must exist)
- **Story 1.1-1.3:** Full UI stack, domain layer, observer pattern

### Anti-Patterns to Reject

- Do NOT delete corrupt files (preserve for debugging)
- Do NOT show error dialogs for missing lists (empty state is valid)
- Do NOT load from UI layer (loading goes through App -> ListManager -> ListStorage)
- Do NOT use raw C file I/O (use std::ifstream with RAII)

### References

- [Source: architecture.md#Application Architecture - Loading Flow]
- [Source: architecture.md#Process Patterns - Error Handling]
- [Source: architecture.md#Integration Points - Startup Flow]
- [Source: epics.md#Story 2.2]

## Dev Agent Record

### Agent Model Used

### Debug Log References

### Completion Notes List

### File List
