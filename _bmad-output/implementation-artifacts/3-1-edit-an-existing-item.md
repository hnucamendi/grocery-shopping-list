# Story 3.1: Edit an Existing Item

Status: ready-for-dev

## Story

As a grocery shopper,
I want to click an item card to edit its name, quantity, or store section,
So that I can fix mistakes or update my list without deleting and re-adding items.

## Acceptance Criteria

1. **Given** items are displayed in the card grid
   **When** the user clicks on an item card
   **Then** the input bar populates with that item's current name, quantity, and store section
   **And** the Add button label changes to "Update"

2. **Given** the input bar is in edit mode with an item's values loaded
   **When** the user modifies the name, quantity, or section and clicks Update (or presses Enter)
   **Then** the item is updated in GroceryList with the new values
   **And** the card in the grid refreshes to show the updated information
   **And** a brief visual confirmation highlight (~500ms) is displayed on the updated card
   **And** the input fields clear, the button reverts to "Add", and focus returns to the item name field

3. **Given** the input bar is in edit mode
   **When** the user modifies values and submits
   **Then** the list is auto-saved to the JSON file via the existing persistence layer

4. **Given** the input bar is in edit mode
   **When** the user clears the name field and submits
   **Then** nothing happens — the edit is not applied, no error dialog shown

## Tasks / Subtasks

- [ ] Task 1: Add edit support to GroceryList domain (AC: #2)
  - [ ] Add updateItem(size_t index, const GroceryItem& updated) to GroceryList
  - [ ] updateItem triggers observer notification (onListChanged)
  - [ ] updateItem triggers auto-save via App wiring
  - [ ] All mutations go through GroceryList methods — never modify GroceryItem directly from UI
- [ ] Task 2: Add edit mode to InputBar (AC: #1, #2, #4)
  - [ ] Track edit state: m_editingIndex (std::optional<size_t>) and m_isEditing bool
  - [ ] Add populateForEdit(size_t index, const GroceryItem& item) method
  - [ ] Populate name, quantity, section fields with item's current values
  - [ ] Change button label to "Update" when editing
  - [ ] On submit in edit mode: call GroceryList::updateItem() instead of addItem()
  - [ ] After submit: clear fields, reset to add mode, return focus to name field
  - [ ] If name is empty on submit: do nothing (no error dialog)
  - [ ] Add cancelEdit() method to reset to add mode without applying changes
- [ ] Task 3: Add click handler to ItemCard (AC: #1)
  - [ ] Bind wxEVT_LEFT_DOWN on ItemCard panel
  - [ ] On click: emit event or callback to MainFrame with item index
  - [ ] MainFrame delegates to InputBar::populateForEdit()
- [ ] Task 4: Add edit confirmation highlight (AC: #2)
  - [ ] Reuse the same highlight mechanism from Story 1.3 (add confirmation)
  - [ ] After updateItem, highlight the updated card for ~500ms
- [ ] Task 5: Write unit tests
  - [ ] Test GroceryList::updateItem() updates correctly and notifies observers
  - [ ] Test updateItem with invalid index (out of range)
  - [ ] Test auto-save triggers on edit
- [ ] Task 6: Build verification
  - [ ] Click item card -> input bar populates, button says "Update"
  - [ ] Modify and submit -> card updates, highlight, fields clear
  - [ ] Submit with empty name -> nothing happens
  - [ ] Verify JSON file updated after edit

## Dev Notes

### Architecture Compliance

**State management rule:** All mutations go through GroceryList methods. The UI layer never modifies a GroceryItem directly. InputBar calls GroceryList::updateItem(), which notifies observers and triggers persistence.

**Edit flow data path:**
```
User clicks ItemCard -> MainFrame receives click event
    -> InputBar::populateForEdit(index, item)
User modifies fields, clicks Update
    -> MainFrame -> GroceryList::updateItem(index, updatedItem)
        -> GroceryList notifies observers (grid refresh + highlight)
        -> App layer triggers ListStorage::save()
```

### InputBar State Machine

```
ADD MODE (default):
  - Button label: "Add"
  - Submit -> GroceryList::addItem()
  - Fields empty

EDIT MODE:
  - Button label: "Update"
  - Submit -> GroceryList::updateItem()
  - Fields pre-populated with selected item
  - Transition to EDIT: click on ItemCard
  - Transition to ADD: after submit, or cancelEdit()
```

### Dependencies on Previous Stories

- **Story 1.2:** InputBar, GroceryItem, GroceryList::addItem()
- **Story 1.3:** ItemCard click handling, highlight mechanism, Observer pattern
- **Story 2.1:** Auto-save persistence (already wired)

### Anti-Patterns to Reject

- Do NOT modify GroceryItem directly from UI code (go through GroceryList)
- Do NOT show error dialogs for empty name (silent no-op)
- Do NOT create a separate edit dialog/modal (inline editing via input bar)
- Do NOT duplicate the highlight animation code (reuse from Story 1.3)

### References

- [Source: architecture.md#Communication Patterns - State Update Rules]
- [Source: ux-design-specification.md#Form Patterns - Edit mode]
- [Source: epics.md#Story 3.1]

## Dev Agent Record

### Agent Model Used

### Debug Log References

### Completion Notes List

### File List
