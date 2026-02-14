# Story 1.3: Section-Grouped Card Display

Status: ready-for-dev

## Story

As a grocery shopper,
I want to see my grocery items displayed in a visual card grid grouped by store section,
So that I can scan my list at a glance and see what I need from each part of the store.

## Acceptance Criteria

1. **Given** items have been added to the current list
   **When** the user views the main window below the input bar
   **Then** items are displayed as visual cards in a scrollable grid area
   **And** cards are grouped under bold section header labels (e.g., "Produce", "Dairy", "Meat")
   **And** all items within a section are displayed together under that section's header

2. **Given** items exist in multiple store sections
   **When** the user views the list
   **Then** each section with items has a visible header spanning full width
   **And** sections without items are not rendered
   **And** the card grid uses wxWrapSizer to reflow cards when the window is resized

3. **Given** a new item has just been added
   **When** the card appears in the grid
   **Then** a brief visual confirmation highlight (~500ms) is displayed on the new card
   **And** the card shows the item name and quantity

4. **Given** no items have been added yet
   **When** the user views the grid area
   **Then** the area is empty with no placeholder text — the always-visible input bar serves as the call to action

5. **Given** the Observer pattern is implemented
   **When** an item is added via GroceryList
   **Then** GroceryList notifies registered observers via IListObserver::onListChanged()
   **And** the ItemGridPanel (as observer) refreshes to display the updated list

## Tasks / Subtasks

- [ ] Task 1: Create ItemCard component (AC: #1, #3)
  - [ ] Create src/ui/ItemCard.h and ItemCard.cpp
  - [ ] wxPanel subclass with vertical wxBoxSizer
  - [ ] Contains: wxStaticBitmap (placeholder image), wxStaticText (name), wxStaticText (quantity)
  - [ ] Load placeholder image from data/images/placeholder.png via wxBitmap
  - [ ] Fixed card dimensions (consistent size across all items)
  - [ ] Set accessible name via SetName() for VoiceOver (item name + quantity)
  - [ ] Add highlight method: setHighlight(bool) — changes background color temporarily
- [ ] Task 2: Create SectionGroup component (AC: #1, #2)
  - [ ] Create src/ui/SectionGroup.h and SectionGroup.cpp
  - [ ] wxStaticText header label — bold font, full width
  - [ ] wxWrapSizer containing ItemCard panels for that section
  - [ ] Method: rebuild(const std::vector<GroceryItem>& sectionItems) — recreates cards
- [ ] Task 3: Create ItemGridPanel (AC: #1, #2, #4, #5)
  - [ ] Create src/ui/ItemGridPanel.h and ItemGridPanel.cpp
  - [ ] wxScrolledWindow subclass — scrollable container for all section groups
  - [ ] Implements IListObserver interface
  - [ ] onListChanged(): rebuild entire grid from GroceryList data
  - [ ] For each StoreSection with items: create SectionGroup with header + cards
  - [ ] Skip sections with zero items (empty sections not rendered)
  - [ ] If no items at all: show empty area (no placeholder text per UX spec)
  - [ ] Vertical wxBoxSizer containing SectionGroup components
- [ ] Task 4: Implement add confirmation highlight (AC: #3)
  - [ ] After onListChanged(), identify the newly added item (last item in list)
  - [ ] Call ItemCard::setHighlight(true) on the new card
  - [ ] Use wxTimer to revert highlight after ~500ms
  - [ ] Highlight = brief background color change on the card's wxPanel
- [ ] Task 5: Create ImageCache (AC: #1)
  - [ ] Create src/ui/ImageCache.h and ImageCache.cpp
  - [ ] std::unordered_map<std::string, wxBitmap> for caching loaded images
  - [ ] getImage(const std::string& path) — loads on first access, returns cached thereafter
  - [ ] getPlaceholder() — returns the fallback placeholder image
  - [ ] Scale images to consistent card dimensions on load
  - [ ] Missing image file: silently return placeholder (no error, no log)
- [ ] Task 6: Integrate ItemGridPanel into MainFrame (AC: #1, #5)
  - [ ] Add ItemGridPanel below InputBar in MainFrame's vertical wxBoxSizer
  - [ ] Register ItemGridPanel as observer on GroceryList
  - [ ] ItemGridPanel receives GroceryList reference for reading items
  - [ ] Grid fills remaining vertical space (wxSizerFlags with Expand and Proportion(1))
- [ ] Task 7: Build verification
  - [ ] Zero warnings on -Wall -Wextra
  - [ ] Add items in different sections — cards appear grouped correctly
  - [ ] Section headers visible, bold, spanning full width
  - [ ] Empty sections not shown
  - [ ] New item gets brief highlight on add
  - [ ] Resize window — cards reflow via wxWrapSizer
  - [ ] Empty list — just empty space below input bar
  - [ ] VoiceOver reads item cards (spot check)

## Dev Notes

### Architecture Compliance

**Observer pattern wiring:**
```
GroceryList::addItem()
    → notifyObservers()
        → ItemGridPanel::onListChanged()
            → rebuild grid from GroceryList::getItems()
            → highlight newest card
```

**Layer boundaries:**
- ItemGridPanel, ItemCard, SectionGroup, ImageCache — all UI layer
- They import from domain (GroceryItem, GroceryList, StoreSection, IListObserver) — never from persistence
- Observer is read-only: onListChanged() re-reads list state, never modifies data

### Component Hierarchy

```
MainFrame (wxFrame)
└── wxBoxSizer (vertical)
    ├── InputBar (wxPanel)
    └── ItemGridPanel (wxScrolledWindow, IListObserver)
        └── wxBoxSizer (vertical)
            ├── SectionGroup "Produce" (wxPanel)
            │   ├── wxStaticText "Produce" (bold header)
            │   └── wxWrapSizer
            │       ├── ItemCard (wxPanel)
            │       └── ItemCard (wxPanel)
            ├── SectionGroup "Dairy" (wxPanel)
            │   ├── wxStaticText "Dairy" (bold header)
            │   └── wxWrapSizer
            │       └── ItemCard (wxPanel)
            └── ... (more sections)
```

### ItemCard Layout

```
┌─────────────┐
│  [image]    │  wxStaticBitmap (placeholder or item image)
│  Item Name  │  wxStaticText
│  Qty: 2 lbs │  wxStaticText
└─────────────┘
```

- Fixed size cards — consistent dimensions
- wxPanel with vertical wxBoxSizer
- Images scaled to card dimensions via ImageCache

### Highlight Implementation

```cpp
void ItemCard::setHighlight(bool on) {
    SetBackgroundColour(on ? wxColour(230, 245, 255) : GetParent()->GetBackgroundColour());
    Refresh();
}

// In ItemGridPanel, after rebuild:
// Start wxTimer for 500ms, on timer event call setHighlight(false) on the highlighted card
```

Use a subtle light blue or system highlight color — nothing garish.

### wxWrapSizer for Responsive Grid

```cpp
auto* wrapSizer = new wxWrapSizer(wxHORIZONTAL);
for (auto& item : sectionItems) {
    auto* card = new ItemCard(sectionPanel, item, imageCache);
    wrapSizer->Add(card, 0, wxALL, 5);
}
```

Cards reflow automatically when window is resized — no manual breakpoint logic needed.

### wxScrolledWindow Setup

```cpp
ItemGridPanel::ItemGridPanel(wxWindow* parent)
    : wxScrolledWindow(parent) {
    SetScrollRate(0, 10);  // vertical scroll only
    // ... create sizer
}
```

### Dependencies on Previous Stories

- **Story 1.1:** MainFrame, project structure, CMakeLists.txt
- **Story 1.2:** GroceryItem, GroceryList, StoreSection, IListObserver, InputBar
- Update CMakeLists.txt to include ItemCard.cpp, SectionGroup.cpp, ItemGridPanel.cpp, ImageCache.cpp

### Anti-Patterns to Reject

- Do NOT show placeholder text or empty state messages (empty grid is the design per UX spec)
- Do NOT render sections with zero items
- Do NOT make observer callbacks modify data (read-only responders)
- Do NOT load images synchronously on every paint (use ImageCache)
- Do NOT use fixed grid columns (use wxWrapSizer for responsive reflow)
- Do NOT skip SetName() on custom card panels (accessibility)

### References

- [Source: architecture.md#Application Architecture - Observer Pattern]
- [Source: architecture.md#Structure Patterns - Project Organization]
- [Source: ux-design-specification.md#Custom Components - Item Card]
- [Source: ux-design-specification.md#Custom Components - Section Group]
- [Source: ux-design-specification.md#Custom Components - Add Confirmation Feedback]
- [Source: ux-design-specification.md#Responsive Strategy]
- [Source: ux-design-specification.md#Empty States]
- [Source: epics.md#Story 1.3]

## Dev Agent Record

### Agent Model Used

### Debug Log References

### Completion Notes List

### File List
