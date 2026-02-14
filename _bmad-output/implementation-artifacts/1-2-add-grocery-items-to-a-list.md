# Story 1.2: Add Grocery Items to a List

Status: ready-for-dev

## Story

As a grocery shopper,
I want to add items to my list with a name, quantity, and store section,
So that I can build my weekly grocery list quickly and with organized categorization.

## Acceptance Criteria

1. **Given** the application is running with an empty list
   **When** the user views the main window
   **Then** an always-visible input bar is displayed at the top with: item name field, quantity field, store section dropdown, and Add button
   **And** the section dropdown contains predefined options: Produce, Dairy, Meat, Frozen, Bakery, Pantry, Household

2. **Given** the input bar is visible and ready
   **When** the user types an item name, quantity, selects a section, and clicks Add (or presses Enter)
   **Then** a new grocery item is created with the provided name, quantity, and store section
   **And** the input fields clear and focus returns to the item name field for rapid re-entry

3. **Given** the input bar is visible
   **When** the user leaves the quantity field empty and adds an item
   **Then** the quantity defaults to "1"

4. **Given** the input bar is visible
   **When** the user leaves the section dropdown at its default
   **Then** the first section option is used as the default

5. **Given** the input bar is visible
   **When** the user tries to add an item with an empty name
   **Then** nothing happens — no item is created, no error dialog shown

6. **Given** the domain layer is implemented
   **When** inspecting the code
   **Then** GroceryItem class exists with m_name, m_quantity, m_section members
   **And** StoreSection is an enum class with PascalCase values
   **And** GroceryList class manages items via addItem() and mutations go through GroceryList methods only
   **And** domain layer has zero dependencies on UI or persistence layers

## Tasks / Subtasks

- [ ] Task 1: Create StoreSection enum (AC: #1, #6)
  - [ ] Create src/domain/StoreSection.h
  - [ ] Define enum class StoreSection: Produce, Dairy, Meat, Frozen, Bakery, Pantry, Household
  - [ ] Create src/domain/StoreSection.cpp
  - [ ] Add toString(StoreSection) -> std::string (PascalCase display names)
  - [ ] Add fromString(const std::string&) -> StoreSection (for deserialization later)
  - [ ] Add getAllSections() -> std::vector<StoreSection> (for dropdown population)
- [ ] Task 2: Create GroceryItem class (AC: #6)
  - [ ] Create src/domain/GroceryItem.h and GroceryItem.cpp
  - [ ] Members: m_name (std::string), m_quantity (std::string), m_section (StoreSection)
  - [ ] Constructor: GroceryItem(std::string name, std::string quantity, StoreSection section)
  - [ ] Getters: getName(), getQuantity(), getSection()
  - [ ] No setters — items are immutable value objects (edits create new items via GroceryList::updateItem)
- [ ] Task 3: Create GroceryList class (AC: #6)
  - [ ] Create src/domain/GroceryList.h and GroceryList.cpp
  - [ ] Member: m_items (std::vector<GroceryItem>)
  - [ ] Member: m_createdDate (std::string, YYYY-MM-DD format)
  - [ ] addItem(GroceryItem item) — appends item, notifies observers
  - [ ] getItems() -> const std::vector<GroceryItem>&
  - [ ] getItemsBySection(StoreSection) -> std::vector<GroceryItem> (filtered)
  - [ ] getCreatedDate() -> std::string
- [ ] Task 4: Create IListObserver interface (AC: #6)
  - [ ] Create src/domain/IListObserver.h (pure interface, no .cpp)
  - [ ] Single method: virtual void onListChanged() = 0
  - [ ] Virtual destructor: virtual ~IListObserver() = default
  - [ ] Add addObserver(IListObserver*) and removeObserver(IListObserver*) to GroceryList
  - [ ] GroceryList::addItem() calls onListChanged() on all registered observers
- [ ] Task 5: Create InputBar UI component (AC: #1, #2, #3, #4, #5)
  - [ ] Create src/ui/InputBar.h and InputBar.cpp
  - [ ] Horizontal wxBoxSizer layout: name field | quantity field | section dropdown | Add button
  - [ ] wxTextCtrl for item name (placeholder: "Item name")
  - [ ] wxTextCtrl for quantity (placeholder: "Qty")
  - [ ] wxChoice for section — populated from StoreSection::getAllSections()
  - [ ] wxButton labeled "Add"
  - [ ] Bind Enter key (wxEVT_TEXT_ENTER) on name and quantity fields to submit
  - [ ] Bind Add button click to submit
  - [ ] On submit: validate name not empty, default quantity to "1" if empty, create GroceryItem, call GroceryList::addItem()
  - [ ] After submit: clear name and quantity fields, reset section to first option, set focus to name field
  - [ ] If name empty on submit: do nothing (silent no-op)
- [ ] Task 6: Integrate InputBar into MainFrame (AC: #1)
  - [ ] Add InputBar as the top element in MainFrame's vertical wxBoxSizer
  - [ ] Pass GroceryList reference to InputBar for addItem() calls
- [ ] Task 7: Write domain unit tests (AC: #6)
  - [ ] Create tests/domain/StoreSection_test.cpp — test toString/fromString roundtrip, getAllSections
  - [ ] Create tests/domain/GroceryItem_test.cpp — test construction, getters
  - [ ] Create tests/domain/GroceryList_test.cpp — test addItem, getItems, getItemsBySection, observer notification
- [ ] Task 8: Build verification
  - [ ] Zero warnings on -Wall -Wextra
  - [ ] All unit tests pass
  - [ ] Launch app, type name/qty/section, click Add — item created (no display yet, that's Story 1.3)
  - [ ] Verify Enter key submits, fields clear, focus returns to name

## Dev Notes

### Architecture Compliance

**Domain layer independence (strictly enforced):**
- GroceryItem, GroceryList, StoreSection, IListObserver — ZERO includes from UI or persistence
- Only STL headers allowed in domain layer
- Domain defines IListObserver interface; UI implements it

**All mutations through GroceryList:**
- UI calls GroceryList::addItem() — never constructs items and manages them independently
- GroceryList owns the items vector
- Every mutation notifies observers

### Naming Conventions

- **Classes:** PascalCase — `GroceryItem`, `GroceryList`, `StoreSection`, `InputBar`
- **Enum values:** PascalCase — `StoreSection::Produce`, `StoreSection::Dairy`
- **Member functions:** camelCase — `addItem()`, `getName()`, `getItemsBySection()`
- **Member variables:** m_ prefix — `m_name`, `m_quantity`, `m_section`, `m_items`
- **Files:** PascalCase.h/.cpp matching class name

### StoreSection Enum Definition

```cpp
enum class StoreSection {
    Produce,
    Dairy,
    Meat,
    Frozen,
    Bakery,
    Pantry,
    Household
};
```

### InputBar Layout (from UX spec)

```
[Item name_______] [Qty__] [Section ▾] [Add]
```

- Always visible at top of window
- Keyboard flow: Tab between fields, Enter to submit
- wxTextCtrl with wxTE_PROCESS_ENTER style for Enter key handling
- After submit: fields clear, focus to name field for rapid re-entry

### GroceryItem Design Decision

- quantity is std::string, NOT numeric — user types "2 lbs", "1 bunch", "3"
- Items are value objects — no setters, no ID. Identified by index in GroceryList's vector
- imagePath is optional (std::string, empty if no image) — not needed for this story but include the member for forward compatibility per architecture doc

### Observer Pattern Implementation

```cpp
// IListObserver.h
class IListObserver {
public:
    virtual ~IListObserver() = default;
    virtual void onListChanged() = 0;
};

// In GroceryList
void GroceryList::addItem(GroceryItem item) {
    m_items.push_back(std::move(item));
    notifyObservers();
}

void GroceryList::notifyObservers() {
    for (auto* observer : m_observers) {
        observer->onListChanged();
    }
}
```

### Dependencies on Previous Stories

- **Story 1.1:** Project structure, CMakeLists.txt, App.h/.cpp, MainFrame (must exist)
- Update CMakeLists.txt to include new source files in both main and test targets

### Anti-Patterns to Reject

- Do NOT make GroceryItem mutable with setters (immutable value object)
- Do NOT use int/float for quantity (use std::string — free text)
- Do NOT show error dialogs for empty name (silent no-op)
- Do NOT use snake_case for C++ functions or member variables
- Do NOT import wxWidgets headers in domain layer files
- Do NOT manage items outside of GroceryList (all mutations through GroceryList methods)

### References

- [Source: architecture.md#Application Architecture]
- [Source: architecture.md#Communication Patterns - Observer Interface]
- [Source: architecture.md#Naming Patterns]
- [Source: ux-design-specification.md#Form Patterns]
- [Source: ux-design-specification.md#Defining Experience - Experience Mechanics]
- [Source: epics.md#Story 1.2]

## Dev Agent Record

### Agent Model Used

### Debug Log References

### Completion Notes List

### File List
