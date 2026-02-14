# Story 4.2: Read-Only Historical List View

Status: ready-for-dev

## Story

As a grocery shopper,
I want to browse a past list's full contents without accidentally modifying it,
So that I can recall what I bought previously while keeping historical data intact.

## Acceptance Criteria

1. **Given** the user has selected a historical list from the dropdown
   **When** the past list is displayed
   **Then** all items from that list are shown in the section-grouped card grid with correct names, quantities, and sections
   **And** the user can scroll through the full list

2. **Given** a historical list is being displayed
   **When** the user views the input bar
   **Then** the input bar is disabled (greyed out fields, inactive Add button)
   **And** it is visually clear that this is a read-only view

3. **Given** a historical list is being displayed
   **When** the user right-clicks an item card
   **Then** no context menu appears — delete is not available for historical items

4. **Given** a historical list is being displayed
   **When** the user clicks an item card
   **Then** no edit mode is triggered — the input bar remains disabled

5. **Given** the user is viewing a historical list
   **When** the user selects "Current List" from the dropdown
   **Then** the input bar is re-enabled and the current list is displayed
   **And** the user can resume adding, editing, and deleting items

## Tasks / Subtasks

- [ ] Task 1: Add read-only mode to InputBar (AC: #2, #5)
  - [ ] Add setReadOnly(bool readOnly) method to InputBar
  - [ ] When read-only: disable all text fields (wxTextCtrl::Disable())
  - [ ] When read-only: disable section dropdown (wxChoice::Disable())
  - [ ] When read-only: disable Add/Update button
  - [ ] When re-enabled: restore all controls to active state
  - [ ] If in edit mode when switching to read-only: cancel edit first
- [ ] Task 2: Disable item card interactions in read-only mode (AC: #3, #4)
  - [ ] Add read-only flag to ItemGridPanel or MainFrame
  - [ ] When read-only: ItemCard left-click does nothing (no edit mode triggered)
  - [ ] When read-only: ItemCard right-click does nothing (no context menu)
  - [ ] When re-enabled: restore normal click/right-click behavior
- [ ] Task 3: Wire read-only mode to list switching (AC: #1, #5)
  - [ ] When historical list selected: MainFrame calls InputBar::setReadOnly(true)
  - [ ] When historical list selected: set grid to read-only mode
  - [ ] When "Current List" selected: InputBar::setReadOnly(false), grid to normal mode
  - [ ] Grid displays historical list items using same section-grouped layout
- [ ] Task 4: Visual clarity for read-only state (AC: #2)
  - [ ] Greyed-out input controls provide visual distinction (wxWidgets handles this natively)
  - [ ] No additional visual indicators needed — disabled controls are sufficient per UX spec
- [ ] Task 5: Build verification
  - [ ] Select historical list -> input bar greyed out
  - [ ] Click item card in historical view -> nothing happens
  - [ ] Right-click item card in historical view -> no context menu
  - [ ] Select "Current List" -> input bar re-enabled, full interaction restored
  - [ ] Verify historical list data displayed correctly (all items, sections)

## Dev Notes

### Architecture Compliance

**Read-only mode is a UI-only concern.** No changes to domain or persistence layers. The historical GroceryList is loaded normally — the UI layer simply disables interaction.

**Mode state management:**
```
ACTIVE MODE (Current List):
  - InputBar: enabled
  - ItemCard clicks: trigger edit mode
  - ItemCard right-clicks: show context menu
  - Auto-save: active

READ-ONLY MODE (Historical List):
  - InputBar: disabled (greyed out)
  - ItemCard clicks: no-op
  - ItemCard right-clicks: no-op
  - Auto-save: not applicable (no mutations possible)
```

### wxWidgets Disable Pattern

```cpp
// Disable controls
m_nameField->Disable();    // Greyed out text field
m_qtyField->Disable();
m_sectionChoice->Disable();
m_addButton->Disable();

// Re-enable
m_nameField->Enable();
m_qtyField->Enable();
m_sectionChoice->Enable();
m_addButton->Enable();
```

wxWidgets natively renders disabled controls as greyed out on macOS — no custom styling needed.

### Dependencies on Previous Stories

- **Story 4.1:** History dropdown, list switching mechanism, ListManager
- **Story 3.1:** Edit mode (must cancel on switch to read-only)
- **Story 3.2:** Context menu (must suppress in read-only mode)
- **Story 1.2-1.3:** InputBar, ItemCard, ItemGridPanel

### Anti-Patterns to Reject

- Do NOT hide the input bar for historical lists (disable/grey out per UX spec)
- Do NOT create a separate "read-only view" component (reuse existing grid with disabled interactions)
- Do NOT modify domain or persistence layers for read-only mode (UI concern only)
- Do NOT show "read-only" text or badge — disabled controls are the visual indicator

### References

- [Source: ux-design-specification.md#Navigation Patterns - Read-only indicator]
- [Source: ux-design-specification.md#Empty States]
- [Source: epics.md#Story 4.2]

## Dev Agent Record

### Agent Model Used

### Debug Log References

### Completion Notes List

### File List
