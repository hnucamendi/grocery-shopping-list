# Story 3.2: Delete an Item

Status: ready-for-dev

## Story

As a grocery shopper,
I want to remove an item from my list,
So that I can clean up items I no longer need.

## Acceptance Criteria

1. **Given** items are displayed in the card grid
   **When** the user right-clicks on an item card
   **Then** a context menu appears with a "Delete" option

2. **Given** the context menu is showing on an item card
   **When** the user clicks "Delete"
   **Then** the item is removed from GroceryList immediately
   **And** the card disappears from the grid
   **And** the list is auto-saved via the existing persistence layer

3. **Given** an item is deleted from a section
   **When** it was the last item in that section
   **Then** the section header is also removed from the grid (empty sections are not rendered)

4. **Given** an item is deleted
   **When** the deletion completes
   **Then** no confirmation dialog is shown — the item is simply removed
   **And** there is no undo mechanism

## Tasks / Subtasks

- [ ] Task 1: Add removeItem to GroceryList domain (AC: #2)
  - [ ] Add removeItem(size_t index) to GroceryList
  - [ ] removeItem triggers observer notification (onListChanged)
  - [ ] removeItem triggers auto-save via App wiring
- [ ] Task 2: Add right-click context menu to ItemCard (AC: #1, #4)
  - [ ] Bind wxEVT_RIGHT_DOWN on ItemCard panel
  - [ ] Create wxMenu with single "Delete" option
  - [ ] Use PopupMenu() to show context menu at click position
  - [ ] No confirmation dialog — direct delete on click
- [ ] Task 3: Wire delete action (AC: #2, #3)
  - [ ] Context menu "Delete" handler calls MainFrame with item index
  - [ ] MainFrame calls GroceryList::removeItem(index)
  - [ ] Observer notification triggers grid rebuild
  - [ ] Grid rebuild naturally excludes empty sections (existing behavior from Story 1.3)
- [ ] Task 4: Handle edit mode edge case (AC: #2)
  - [ ] If user is editing an item (InputBar in edit mode) and deletes that same item via right-click:
    cancel edit mode and clear input fields
  - [ ] If user deletes a different item while editing: adjust editing index if needed
- [ ] Task 5: Write unit tests
  - [ ] Test GroceryList::removeItem() removes correctly and notifies observers
  - [ ] Test removeItem with last item in section (section should have no items)
  - [ ] Test removeItem with invalid index
  - [ ] Test auto-save triggers on delete
- [ ] Task 6: Build verification
  - [ ] Right-click item -> context menu with "Delete"
  - [ ] Click Delete -> item removed, grid updates, no dialog
  - [ ] Delete last item in section -> section header disappears
  - [ ] Verify JSON file updated after delete

## Dev Notes

### Architecture Compliance

**Delete flow data path:**
```
User right-clicks ItemCard -> wxEVT_RIGHT_DOWN
    -> PopupMenu with "Delete" option
User clicks "Delete"
    -> MainFrame -> GroceryList::removeItem(index)
        -> GroceryList notifies observers (grid rebuilds)
        -> App layer triggers ListStorage::save()
```

**Empty section handling:** The grid rebuild logic from Story 1.3 already skips sections with no items. Deleting the last item in a section should work automatically — the section group simply won't be rendered on the next onListChanged refresh.

### Context Menu Implementation

```cpp
// In ItemCard's right-click handler
void ItemCard::onRightClick(wxMouseEvent& event) {
    wxMenu menu;
    menu.Append(wxID_DELETE, "Delete");
    // Bind and PopupMenu
}
```

- Use wxID_DELETE as the menu item ID (standard wxWidgets ID for delete)
- Single menu item only — no other options needed

### Dependencies on Previous Stories

- **Story 1.3:** ItemCard, grid layout, section rendering (empty section exclusion)
- **Story 3.1:** Edit mode (need to handle cancel-on-delete edge case)
- **Story 2.1:** Auto-save persistence

### Anti-Patterns to Reject

- Do NOT show a confirmation dialog before delete (per UX spec)
- Do NOT implement undo (per UX spec — out of scope)
- Do NOT modify items directly from UI (go through GroceryList::removeItem)
- Do NOT leave orphaned section headers when all items are deleted

### References

- [Source: ux-design-specification.md#Button Hierarchy - Destructive action]
- [Source: ux-design-specification.md#Feedback Patterns - Success (item deleted)]
- [Source: epics.md#Story 3.2]

## Dev Agent Record

### Agent Model Used

### Debug Log References

### Completion Notes List

### File List
