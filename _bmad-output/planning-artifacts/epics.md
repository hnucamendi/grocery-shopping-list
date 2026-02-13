---
stepsCompleted: [step-01-validate-prerequisites, step-02-design-epics, step-03-create-stories, step-04-final-validation]
inputDocuments: ['_bmad-output/planning-artifacts/prd.md', '_bmad-output/planning-artifacts/architecture.md', '_bmad-output/planning-artifacts/ux-design-specification.md']
---

# grocery-shopping-list - Epic Breakdown

## Overview

This document provides the complete epic and story breakdown for grocery-shopping-list, decomposing the requirements from the PRD, UX Design if it exists, and Architecture requirements into implementable stories.

## Requirements Inventory

### Functional Requirements

FR1: User can create a new grocery list
FR2: User can add an item to the current list with a name
FR3: User can specify a quantity for each item
FR4: User can assign a store section to each item
FR5: User can edit an existing item's name, quantity, or store section
FR6: User can delete an item from the current list
FR7: User can view items grouped by store section
FR8: User can see all items within a store section displayed together
FR9: User can select from a predefined set of store sections when adding items (produce, dairy, meat, frozen, bakery, pantry, household, etc.)
FR10: System automatically saves the current list to local storage
FR11: System loads the most recent list on application startup
FR12: System preserves all item data (name, quantity, store section) across application restarts
FR13: User can view a list of all previously saved lists
FR14: User can browse a past list and see its full contents
FR15: System stores lists with their creation date for identification
FR16: User can distinguish between the current active list and historical lists
FR17: User can launch the application as a standalone macOS desktop window
FR18: User can add items through a text input interface
FR19: User can view the complete list in a single scrollable view
FR20: User can visually distinguish between different store sections in the list display

### NonFunctional Requirements

NFR1: System preserves list data accurately across application restarts with no data loss or corruption
NFR2: System handles unexpected shutdown gracefully without corrupting saved data
NFR3: UI follows clean, minimal design principles -- no visual clutter, clear information hierarchy
NFR4: Adding an item to the list requires no more than 3 user interactions (name, quantity, section)
NFR5: List display is scannable at a glance with clear visual grouping by store section
NFR6: Codebase compiles with zero warnings on `-Wall -Wextra`
NFR7: No raw memory management (`new`/`delete`) unless intentionally exploring manual memory management
NFR8: Architecture supports addition of new feature epics without rewriting existing modules

### Additional Requirements

**From Architecture:**
- Starter template: Manual CMake + Homebrew wxWidgets (no template -- hand-written CMakeLists.txt)
- C++17 standard with Apple Clang compiler
- wxWidgets 3.2.9 (via Homebrew) for GUI framework
- nlohmann/json 3.12.0 (header-only, via CMake FetchContent) for data serialization
- Catch2 (header-only, via CMake FetchContent) for unit testing
- Layered architecture: UI → Domain → Persistence (strictly enforced boundaries)
- Observer pattern (IListObserver) for UI-domain communication
- Atomic write pattern (temp file + rename) for data integrity (NFR1-2)
- One JSON file per list, named by date (YYYY-MM-DD.json) stored in data/lists/
- Image management: optional imagePath per item, wxBitmap cache, placeholder fallback
- Project structure: src/app/, src/domain/, src/persistence/, src/ui/, tests/, data/
- Compiler flags: -Wall -Wextra enforced via CMake
- All mutations through GroceryList methods; GroceryList notifies observers + triggers persistence
- ListManager coordinates active list and delegates to ListStorage

**From UX Design:**
- Keyboard-driven input: tab/enter flow for rapid item entry
- Item card grid display with images (wxStaticBitmap + wxStaticText)
- Section grouping with bold section header labels spanning full width
- Always-visible input bar at top (name field, quantity field, section dropdown, add button)
- Add confirmation feedback: brief ~500ms background highlight on new card
- Edit flow: click item card to populate input fields, Add button becomes "Update"
- Delete flow: right-click context menu on item cards
- List history selector: wxChoice dropdown in title area (Current List + dated entries)
- Read-only mode for historical lists: input bar disabled (greyed out)
- Minimum window size: 600x400
- Empty sections not rendered; empty grid shows nothing (input bar is the call-to-action)
- VoiceOver accessibility: SetName() on custom card panels
- wxWrapSizer for responsive card grid layout
- Enter key or Add button submits; fields clear and focus returns to name field after submit
- Quantity defaults to "1" if empty; section defaults to first dropdown option

### FR Coverage Map

FR1: Epic 1 - Create new grocery list
FR2: Epic 1 - Add item with name
FR3: Epic 1 - Specify quantity
FR4: Epic 1 - Assign store section
FR5: Epic 3 - Edit item
FR6: Epic 3 - Delete item
FR7: Epic 1 - View items grouped by section
FR8: Epic 1 - Items within section displayed together
FR9: Epic 1 - Predefined store sections
FR10: Epic 2 - Auto-save to local storage
FR11: Epic 2 - Load most recent list on startup
FR12: Epic 2 - Preserve data across restarts
FR13: Epic 4 - View all previously saved lists
FR14: Epic 4 - Browse past list contents
FR15: Epic 4 - Lists stored with creation date
FR16: Epic 4 - Distinguish current vs historical
FR17: Epic 1 - Standalone macOS desktop window
FR18: Epic 1 - Text input interface
FR19: Epic 1 - Single scrollable view
FR20: Epic 1 - Visual section distinction

## Epic List

### Epic 1: Build a Grocery List
User can launch the app on macOS, add grocery items with name, quantity, and store section, and see them displayed in an organized card grid grouped by store section.
**FRs covered:** FR1, FR2, FR3, FR4, FR7, FR8, FR9, FR17, FR18, FR19, FR20

### Epic 2: Save & Load Lists
Lists persist automatically -- every add triggers a save, and the app loads the most recent list on startup.
**FRs covered:** FR10, FR11, FR12

### Epic 3: Edit & Delete Items
User can modify an existing item's name, quantity, or section, and can remove items from the list.
**FRs covered:** FR5, FR6

### Epic 4: Browse List History
User can view all previously saved lists, browse their full contents by date, and distinguish between the current list and past lists.
**FRs covered:** FR13, FR14, FR15, FR16

## Epic 1: Build a Grocery List

User can launch the app on macOS, add grocery items with name, quantity, and store section, and see them displayed in an organized card grid grouped by store section.

### Story 1.1: Project Setup & Application Window

As a developer/user,
I want to launch a macOS desktop application with a clean empty window,
So that I have a working application foundation to build grocery list features on.

**Acceptance Criteria:**

**Given** the project is cloned and dependencies are installed (brew install wxwidgets cmake)
**When** the developer runs `cmake -B build && cmake --build build`
**Then** the project compiles with zero warnings on `-Wall -Wextra`
**And** a single executable is produced

**Given** the application executable exists
**When** the user launches the application
**Then** a standalone macOS desktop window appears with title "Grocery Shopping List"
**And** the window has a minimum size of 600x400
**And** the window uses native macOS controls and styling

**Given** the project is built
**When** the developer inspects the project structure
**Then** the directory structure matches the architecture spec: src/app/, src/domain/, src/persistence/, src/ui/, tests/domain/, tests/persistence/, data/lists/, data/images/
**And** CMakeLists.txt uses find_package(wxWidgets) and FetchContent for nlohmann/json and Catch2
**And** C++17 standard is configured

**Given** the test target exists
**When** the developer runs the test executable
**Then** Catch2 test framework runs successfully (even if only a placeholder test exists)

### Story 1.2: Add Grocery Items to a List

As a grocery shopper,
I want to add items to my list with a name, quantity, and store section,
So that I can build my weekly grocery list quickly and with organized categorization.

**Acceptance Criteria:**

**Given** the application is running with an empty list
**When** the user views the main window
**Then** an always-visible input bar is displayed at the top with: item name field, quantity field, store section dropdown, and Add button
**And** the section dropdown contains predefined options: Produce, Dairy, Meat, Frozen, Bakery, Pantry, Household

**Given** the input bar is visible and ready
**When** the user types an item name, quantity, selects a section, and clicks Add (or presses Enter)
**Then** a new grocery item is created with the provided name, quantity, and store section
**And** the input fields clear and focus returns to the item name field for rapid re-entry

**Given** the input bar is visible
**When** the user leaves the quantity field empty and adds an item
**Then** the quantity defaults to "1"

**Given** the input bar is visible
**When** the user leaves the section dropdown at its default
**Then** the first section option is used as the default

**Given** the input bar is visible
**When** the user tries to add an item with an empty name
**Then** nothing happens -- no item is created, no error dialog shown

**Given** the domain layer is implemented
**When** inspecting the code
**Then** GroceryItem class exists with m_name, m_quantity, m_section members
**And** StoreSection is an enum class with PascalCase values
**And** GroceryList class manages items via addItem() and mutations go through GroceryList methods only
**And** domain layer has zero dependencies on UI or persistence layers

### Story 1.3: Section-Grouped Card Display

As a grocery shopper,
I want to see my grocery items displayed in a visual card grid grouped by store section,
So that I can scan my list at a glance and see what I need from each part of the store.

**Acceptance Criteria:**

**Given** items have been added to the current list
**When** the user views the main window below the input bar
**Then** items are displayed as visual cards in a scrollable grid area
**And** cards are grouped under bold section header labels (e.g., "Produce", "Dairy", "Meat")
**And** all items within a section are displayed together under that section's header

**Given** items exist in multiple store sections
**When** the user views the list
**Then** each section with items has a visible header spanning full width
**And** sections without items are not rendered
**And** the card grid uses wxWrapSizer to reflow cards when the window is resized

**Given** a new item has just been added
**When** the card appears in the grid
**Then** a brief visual confirmation highlight (~500ms) is displayed on the new card
**And** the card shows the item name and quantity

**Given** no items have been added yet
**When** the user views the grid area
**Then** the area is empty with no placeholder text -- the always-visible input bar serves as the call to action

**Given** the Observer pattern is implemented
**When** an item is added via GroceryList
**Then** GroceryList notifies registered observers via IListObserver::onListChanged()
**And** the ItemGridPanel (as observer) refreshes to display the updated list

## Epic 2: Save & Load Lists

Lists persist automatically -- every add triggers a save, and the app loads the most recent list on startup.

### Story 2.1: JSON Serialization & Auto-Save

As a grocery shopper,
I want my grocery list to be automatically saved every time I add an item,
So that I never lose my list if I close the app or it shuts down unexpectedly.

**Acceptance Criteria:**

**Given** the user adds an item to the current list
**When** the item is created in GroceryList
**Then** the entire list is automatically saved to a JSON file in data/lists/
**And** the file is named by date (e.g., 2026-02-13.json)
**And** no manual save action is required from the user

**Given** a save is triggered
**When** the system writes the list to disk
**Then** the atomic write pattern is used: write to a temp file first, then rename to the final filename
**And** an interrupted write does not corrupt the existing saved file

**Given** a list is saved to JSON
**When** inspecting the file contents
**Then** the JSON structure matches the architecture spec: `created_date`, `items` array with `name`, `quantity`, `store_section` fields
**And** JSON field names use snake_case
**And** store_section values are lowercase strings matching the enum

**Given** the data/lists/ directory does not exist
**When** the first save is triggered
**Then** the directory is created automatically

**Given** the persistence layer is implemented
**When** inspecting the code
**Then** JsonSerializer handles struct-to-JSON mapping using nlohmann/json
**And** ListStorage handles file I/O with atomic writes
**And** persistence layer depends on domain layer only -- no wxWidgets imports
**And** unit tests exist for JsonSerializer and ListStorage

### Story 2.2: Load List on Startup

As a grocery shopper,
I want my most recent list to appear when I open the app,
So that I can pick up right where I left off without any extra steps.

**Acceptance Criteria:**

**Given** one or more saved list files exist in data/lists/
**When** the user launches the application
**Then** the most recent list (by date in filename) is loaded automatically
**And** all items are displayed in the section-grouped card grid with correct names, quantities, and sections

**Given** no saved list files exist in data/lists/
**When** the user launches the application
**Then** the app displays an empty grid with the input bar ready
**And** no error is shown

**Given** a saved list file is corrupt or has invalid JSON
**When** the application attempts to load it
**Then** a warning is logged via wxLogWarning
**And** the corrupt file is not deleted
**And** the app falls back to displaying an empty grid or the next most recent valid list

**Given** all item data was saved in a previous session
**When** the list is loaded on startup
**Then** every item's name, quantity, and store section is preserved exactly as it was saved (FR12)

## Epic 3: Edit & Delete Items

User can modify an existing item's name, quantity, or section, and can remove items from the list.

### Story 3.1: Edit an Existing Item

As a grocery shopper,
I want to click an item card to edit its name, quantity, or store section,
So that I can fix mistakes or update my list without deleting and re-adding items.

**Acceptance Criteria:**

**Given** items are displayed in the card grid
**When** the user clicks on an item card
**Then** the input bar populates with that item's current name, quantity, and store section
**And** the Add button label changes to "Update"

**Given** the input bar is in edit mode with an item's values loaded
**When** the user modifies the name, quantity, or section and clicks Update (or presses Enter)
**Then** the item is updated in GroceryList with the new values
**And** the card in the grid refreshes to show the updated information
**And** a brief visual confirmation highlight (~500ms) is displayed on the updated card
**And** the input fields clear, the button reverts to "Add", and focus returns to the item name field

**Given** the input bar is in edit mode
**When** the user modifies values and submits
**Then** the list is auto-saved to the JSON file via the existing persistence layer

**Given** the input bar is in edit mode
**When** the user clears the name field and submits
**Then** nothing happens -- the edit is not applied, no error dialog shown

### Story 3.2: Delete an Item

As a grocery shopper,
I want to remove an item from my list,
So that I can clean up items I no longer need.

**Acceptance Criteria:**

**Given** items are displayed in the card grid
**When** the user right-clicks on an item card
**Then** a context menu appears with a "Delete" option

**Given** the context menu is showing on an item card
**When** the user clicks "Delete"
**Then** the item is removed from GroceryList immediately
**And** the card disappears from the grid
**And** the list is auto-saved via the existing persistence layer

**Given** an item is deleted from a section
**When** it was the last item in that section
**Then** the section header is also removed from the grid (empty sections are not rendered)

**Given** an item is deleted
**When** the deletion completes
**Then** no confirmation dialog is shown -- the item is simply removed
**And** there is no undo mechanism

## Epic 4: Browse List History

User can view all previously saved lists, browse their full contents by date, and distinguish between the current list and past lists.

### Story 4.1: List History Selector

As a grocery shopper,
I want to see a dropdown of all my previously saved lists organized by date,
So that I can quickly find and select a past list to review.

**Acceptance Criteria:**

**Given** the application is running and saved list files exist in data/lists/
**When** the user views the title area of the main window
**Then** a wxChoice dropdown is visible showing "Current List" as the first option
**And** historical lists are listed by date (most recent first, e.g., "Feb 9, 2026", "Feb 2, 2026")

**Given** the history dropdown is visible
**When** the user clicks the dropdown
**Then** all available list dates from data/lists/ are shown as selectable options
**And** each entry is identified by its creation date from the JSON file

**Given** a historical list is selected from the dropdown
**When** the selection changes
**Then** the grid displays the selected past list's items in the section-grouped card layout

**Given** the user is viewing a historical list
**When** the user selects "Current List" from the dropdown
**Then** the grid returns to displaying the current active list
**And** the input bar is re-enabled

**Given** no historical list files exist
**When** the user views the dropdown
**Then** only "Current List" is shown as an option

### Story 4.2: Read-Only Historical List View

As a grocery shopper,
I want to browse a past list's full contents without accidentally modifying it,
So that I can recall what I bought previously while keeping historical data intact.

**Acceptance Criteria:**

**Given** the user has selected a historical list from the dropdown
**When** the past list is displayed
**Then** all items from that list are shown in the section-grouped card grid with correct names, quantities, and sections
**And** the user can scroll through the full list

**Given** a historical list is being displayed
**When** the user views the input bar
**Then** the input bar is disabled (greyed out fields, inactive Add button)
**And** it is visually clear that this is a read-only view

**Given** a historical list is being displayed
**When** the user right-clicks an item card
**Then** no context menu appears -- delete is not available for historical items

**Given** a historical list is being displayed
**When** the user clicks an item card
**Then** no edit mode is triggered -- the input bar remains disabled

**Given** the user is viewing a historical list
**When** the user selects "Current List" from the dropdown
**Then** the input bar is re-enabled and the current list is displayed
**And** the user can resume adding, editing, and deleting items
