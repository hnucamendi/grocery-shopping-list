---
stepsCompleted: [step-01-init, step-02-discovery, step-03-success, step-04-journeys, step-05-domain, step-06-innovation, step-07-project-type, step-08-scoping, step-09-functional, step-10-nonfunctional, step-11-polish, step-12-complete]
inputDocuments: ['_bmad-output/brainstorming/brainstorming-session-2026-02-12.md']
workflowType: 'prd'
documentCounts:
  briefs: 0
  research: 0
  brainstorming: 1
  projectDocs: 0
classification:
  projectType: desktop_app
  domain: general
  complexity: low
  projectContext: greenfield
---

# Product Requirements Document - grocery-shopping-list

**Author:** Hnucamend
**Date:** 2026-02-12

## Executive Summary

A macOS desktop grocery shopping list application built in C++. The primary purpose is C++ skill acquisition across GUI frameworks, OOP, STL containers, file I/O, and design patterns -- transferable skills toward systems engineering, graphics programming, and other C++ roles.

The app solves a simple, real problem: building a weekly grocery list organized by store section, with historical lookback. One user, one machine, no cloud, no accounts. Design philosophy follows "Ally Bank vibes" -- clean, no-nonsense, data-respecting UI with zero gamification.

**Target User:** Solo developer/user who plans meals weekly and builds a grocery list from those plans.

**Differentiator:** This is a learning vehicle, not a market product. Success is measured by developer growth, not user adoption.

## Success Criteria

### User Success

- Open the app and see a clean, organized grocery list grouped by store section with zero friction
- UI meets the "Ally Bank standard": clean typography, clear information hierarchy, no clutter
- List management feels faster and more organized than phone notes or store apps

### Business Success

- **Learning-first project** -- success is measured by C++ skill acquisition, not product metrics
- No monetization, no user growth targets -- the builder is the user
- Success = transferable C++ skills applicable to systems engineering, graphics programming, and other domains
- After each ticket, the developer can articulate the C++ concepts used and explain when they'd choose a different approach

### Technical Success

- Meaningful reps across C++ fundamentals: OOP, STL containers, design patterns
- **Memory management discipline**: RAII patterns, ownership semantics, zero memory leaks (see NFR7)
- **Compile discipline**: zero warnings on `-Wall -Wextra` (see NFR6)
- GUI framework proficiency (wxWidgets or Qt) -- event handling, layout, rendering
- Core language patterns: file I/O, serialization, sorting/filtering, state management
- **Extensibility**: MVP architecture supports adding Epics 1 and 4 without rewriting existing code (see NFR8)

### Measurable Outcomes

- Complete MVP tickets with demonstrated understanding of each ticket's C++ concepts
- Each ticket produces working, compilable code with zero warnings
- Developer can explain the C++ concept in each ticket and articulate alternative approaches
- Codebase supports adding Epics 1 (Recipes) and 4 (Budget/Prices) without gut-rewrites

## User Journeys

### Journey 1: The Weekly List Builder

**Meet Marco.** He's an intermediate C++ developer who also happens to need groceries. Every week -- usually on a Sunday -- he sits down for a couple of hours to think through what he's eating this week. He's got recipes in his head, bookmarks, maybe a text thread with his partner.

**Opening Scene:** Marco opens the app on his Mac. A clean, empty list greets him -- or maybe last week's list is there for reference. He starts thinking through Monday's dinner. Chicken stir fry. He types: "chicken breast, 2 lbs, meat." Then "broccoli, 1 bunch, produce." Item by item, recipe by recipe, the list grows.

**Rising Action:** As he works through the week's meals, the list organizes itself by store section. Produce groups together, dairy groups together. He adds household items too -- paper towels, dish soap. Each entry is quick: name, quantity, section. No friction.

**Climax:** He finishes the list, scans it once. Everything's there, grouped cleanly. He can see at a glance that he's got 6 produce items, 3 meat items, 4 dairy items. Done.

**Resolution:** Marco closes the app. The list is saved. Later, weeks from now, he opens the app and scrolls through old lists. "What did we eat the first week of February?" It's right there.

### Journey 2: The History Browser

**Same Marco, different day.** It's Thursday. His partner asks "didn't we make that salmon thing a few weeks ago? What did we buy for it?" Marco opens the app, scrolls back through saved lists, finds the week with salmon. Atlantic salmon, 1 lb. Asparagus, 1 bunch. Lemon, 2. Garlic, 1 head. There it is.

The historical list is a lightweight memory system. No search, no tags needed yet. Scroll back and scan. The clean section-grouped layout makes it easy to spot what you're looking for.

### Journey Requirements Summary

- **List creation**: Add items with name, quantity, and store section
- **Section grouping**: Items automatically grouped by store section in the UI
- **Persistence**: Lists saved automatically and survive app restarts
- **List history**: Browse previously created lists by date
- **Clean UI**: Scannable, grouped, no clutter
- **Edit/delete**: Modify or remove items during the building session

## Project Scope & Phased Development

### MVP Strategy

**Approach:** Problem-solving MVP -- the minimum that makes weekly grocery list building useful.

**Resource:** Solo developer, intermediate C++ skill level. Each ticket introduces specific C++ concepts.

### MVP Feature Set (Phase 1)

**Journeys Supported:** Weekly List Builder, History Browser

**Must-Have Capabilities (6 tickets):**

1. Desktop UI shell -- clean macOS window with list display and input controls (GUI framework, event handling)
2. List data model -- items with name, quantity, and store section (classes, STL containers, OOP)
3. Persistent storage -- save lists to local file, load on startup (file I/O, serialization, parsing)
4. Store section grouping -- items displayed grouped by section (enums, sorting, grouping)
5. List history -- browse previously saved lists by date (data management, date handling)
6. Edit/delete items -- modify or remove items during the building session (state management, UI updates)

### Phase 2 (Growth)

- Check-off with "got it" section (in-store shopping support)
- Remaining items count
- Missed item alert at checkout
- Smart ingredient consolidation
- Recipe book CRUD and ingredient lists (Epic 1)
- Item price entry and budget envelopes (Epic 4)

### Phase 3 (Expansion)

- Weekly meal planner (Epic 2)
- Spending dashboard with Ally-style graphs (Epic 5)
- Shared couple experience / live sync (Epic 6)
- Smart history and suggestions (Epic 7)

### Risk Mitigation

**Technical:** GUI framework selection (wxWidgets vs Qt) is the highest-risk early decision. Mitigated by spike/prototype during architecture phase.

**Resource:** Solo developer with limited time. Mitigated by lean MVP scope (6 tickets) and learning-first success criteria -- partial completion still counts as success if concepts are learned.

## Desktop App Specific Requirements

### Platform Support

- **Target:** macOS only
- **Architecture:** Native C++ with GUI framework (wxWidgets or Qt)
- **No cross-platform requirement** -- simplifies build tooling and framework choices
- **macOS minimum version:** TBD during architecture phase

### Technical Architecture

- **Offline-first by design:** All data stored locally, no network dependencies
- **No auto-update mechanism:** User builds from source
- **No system integration:** No notifications, file associations, or menu bar presence
- **Standalone window application:** Single window, standard desktop UI patterns

### Implementation Considerations

- GUI framework selection drives event handling patterns, layout approach, and build system
- Local file storage for persistence (JSON or similar) -- no database needed
- Build system: Makefile or CMake for macOS compilation
- Zero external service dependencies

## Functional Requirements

### List Management

- FR1: User can create a new grocery list
- FR2: User can add an item to the current list with a name
- FR3: User can specify a quantity for each item
- FR4: User can assign a store section to each item
- FR5: User can edit an existing item's name, quantity, or store section
- FR6: User can delete an item from the current list

### Item Organization

- FR7: User can view items grouped by store section
- FR8: User can see all items within a store section displayed together
- FR9: User can select from a predefined set of store sections when adding items (produce, dairy, meat, frozen, bakery, pantry, household, etc.)

### Data Persistence

- FR10: System automatically saves the current list to local storage
- FR11: System loads the most recent list on application startup
- FR12: System preserves all item data (name, quantity, store section) across application restarts

### List History

- FR13: User can view a list of all previously saved lists
- FR14: User can browse a past list and see its full contents
- FR15: System stores lists with their creation date for identification
- FR16: User can distinguish between the current active list and historical lists

### Item Images

- FR21: User can associate a local image with a grocery item for visual recognition
- FR22: System displays a generic placeholder image for items without a user-provided image

### User Interface

- FR17: User can launch the application as a standalone macOS desktop window
- FR18: User can add items through a text input interface
- FR19: User can view the complete list in a single scrollable view
- FR20: User can visually distinguish between different store sections in the list display

## Non-Functional Requirements

### Data Integrity

- NFR1: System preserves list data accurately across application restarts with no data loss or corruption
- NFR2: System handles unexpected shutdown gracefully without corrupting saved data

### Usability

- NFR3: UI follows clean, minimal design principles -- no visual clutter, clear information hierarchy
- NFR4: Adding an item to the list requires no more than 3 user interactions (name, quantity, section)
- NFR5: List display is scannable at a glance with clear visual grouping by store section

### Code Quality

- NFR6: Codebase compiles with zero warnings on `-Wall -Wextra`
- NFR7: No raw memory management (`new`/`delete`) unless intentionally exploring manual memory management
- NFR8: Architecture supports addition of new feature epics without rewriting existing modules
