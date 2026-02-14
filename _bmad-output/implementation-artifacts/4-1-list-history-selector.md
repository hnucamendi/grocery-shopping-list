# Story 4.1: List History Selector

Status: ready-for-dev

## Story

As a grocery shopper,
I want to see a dropdown of all my previously saved lists organized by date,
So that I can quickly find and select a past list to review.

## Acceptance Criteria

1. **Given** the application is running and saved list files exist in data/lists/
   **When** the user views the title area of the main window
   **Then** a wxChoice dropdown is visible showing "Current List" as the first option
   **And** historical lists are listed by date (most recent first, e.g., "Feb 9, 2026", "Feb 2, 2026")

2. **Given** the history dropdown is visible
   **When** the user clicks the dropdown
   **Then** all available list dates from data/lists/ are shown as selectable options
   **And** each entry is identified by its creation date from the JSON file

3. **Given** a historical list is selected from the dropdown
   **When** the selection changes
   **Then** the grid displays the selected past list's items in the section-grouped card layout

4. **Given** the user is viewing a historical list
   **When** the user selects "Current List" from the dropdown
   **Then** the grid returns to displaying the current active list
   **And** the input bar is re-enabled

5. **Given** no historical list files exist
   **When** the user views the dropdown
   **Then** only "Current List" is shown as an option

## Tasks / Subtasks

- [ ] Task 1: Add list discovery to ListManager (AC: #1, #5)
  - [ ] ListManager::getAvailableLists() returns list of dates from ListStorage
  - [ ] Sort dates descending (most recent first)
  - [ ] Filter out current day's list (that's the active list, shown as "Current List")
  - [ ] Return empty vector if no historical lists exist
- [ ] Task 2: Create history dropdown in MainFrame (AC: #1, #2)
  - [ ] Add wxChoice to the title/header area of MainFrame
  - [ ] First option: "Current List" (always present)
  - [ ] Subsequent options: formatted dates (e.g., "Feb 9, 2026") from ListManager
  - [ ] Populate on app startup and refresh when lists change
- [ ] Task 3: Handle list switching (AC: #3, #4)
  - [ ] Bind wxEVT_CHOICE on the history dropdown
  - [ ] On historical list selection: load via ListManager -> ListStorage -> JsonSerializer
  - [ ] Replace grid contents with historical list items
  - [ ] On "Current List" selection: restore active list to grid
  - [ ] If InputBar is in edit mode when switching: cancel edit mode
- [ ] Task 4: Date formatting (AC: #1)
  - [ ] Convert YYYY-MM-DD filename to human-readable format ("Feb 9, 2026")
  - [ ] Use standard C++ date formatting (std::put_time or manual formatting)
- [ ] Task 5: Build verification
  - [ ] Save multiple lists on different dates, verify dropdown shows all
  - [ ] Select historical list -> grid shows correct items
  - [ ] Select "Current List" -> returns to active list
  - [ ] No historical lists -> only "Current List" in dropdown

## Dev Notes

### Architecture Compliance

**ListManager role (from architecture doc):**
- ListManager coordinates active list and delegates to ListStorage
- Provides abstraction between App wiring and persistence
- Handles switching between current and historical lists

**Data flow for list switching:**
```
User selects date from wxChoice
    -> MainFrame receives wxEVT_CHOICE
    -> MainFrame asks ListManager to load historical list
    -> ListManager -> ListStorage::load(date) -> JsonSerializer -> GroceryList
    -> MainFrame displays loaded list in grid (read-only)
```

### Date Display Format

- Filename: `2026-02-09.json`
- Dropdown display: `"Feb 9, 2026"`
- Parse YYYY-MM-DD from filename, format for display

### wxChoice Placement

Per UX spec: wxChoice dropdown in the title area of the main window. Layout:
```
[App Title / "Grocery Shopping List"]  [Current List ▾]
[Input Bar: name | qty | section | Add]
[Scrollable Item Grid]
```

### Dependencies on Previous Stories

- **Story 2.1-2.2:** ListStorage (save/load), JsonSerializer, ListManager
- **Story 1.1-1.3:** MainFrame, grid display, full UI stack

### Anti-Patterns to Reject

- Do NOT use wxComboBox (use wxChoice — read-only dropdown, no text input)
- Do NOT load all historical lists into memory at once (load on demand when selected)
- Do NOT show raw filenames in dropdown (format dates for display)
- Do NOT allow editing while viewing historical list (handled in Story 4.2)

### References

- [Source: ux-design-specification.md#Navigation Patterns - List switching]
- [Source: ux-design-specification.md#Design Direction Decision]
- [Source: architecture.md#Integration Points - ListManager]
- [Source: epics.md#Story 4.1]

## Dev Agent Record

### Agent Model Used

### Debug Log References

### Completion Notes List

### File List
