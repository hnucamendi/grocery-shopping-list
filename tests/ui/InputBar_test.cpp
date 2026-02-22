#include "domain/StoreSection.h"
#define TESTING
#include "domain/GroceryList.h"
#include "ui/InputBar.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <wx/frame.h>
#include <wx/init.h>

TEST_CASE("Test input validation logic") {
  wxInitialize();
  {
    auto frame = new wxFrame(nullptr, wxID_ANY, "Test Parent Frame");
    auto groceryList = std::make_unique<GroceryList>();
    auto *inputBar = new InputBar(frame, groceryList.get());

    SECTION("Empty name prevents adding item") {
      inputBar->setNameForTests("");
      inputBar->setQuantityForTests("");
      inputBar->setSectionForTests(1);

      inputBar->clickAddForTests();

      REQUIRE(groceryList->getItems().empty());
    }
    SECTION("Empty Quantity prevents adding item") {
      inputBar->setNameForTests("banana");
      inputBar->setQuantityForTests("");
      inputBar->setSectionForTests(1);

      inputBar->clickAddForTests();

      REQUIRE(groceryList->getItems().empty());
    }
    SECTION("Empty Section prevents adding item") {
      inputBar->setNameForTests("banana");
      inputBar->setQuantityForTests("1 lbs");
      inputBar->setSectionForTests(0);

      inputBar->clickAddForTests();

      REQUIRE(groceryList->getItems().empty());
    }
    SECTION("All inputs invalid") {
      inputBar->setNameForTests("");
      inputBar->setQuantityForTests("");
      inputBar->setSectionForTests(0);

      inputBar->clickAddForTests();

      REQUIRE(groceryList->getItems().empty());
    }
    SECTION("Valid inputs add item") {
      inputBar->setNameForTests("banana");
      inputBar->setQuantityForTests("1 lbs");
      inputBar->setSectionForTests(1);

      inputBar->clickAddForTests();

      REQUIRE(!groceryList->getItems().empty());
      REQUIRE(groceryList->getItems().size() == 1);
      REQUIRE(groceryList->getItems()[0].getName() == "banana");
      REQUIRE(groceryList->getItems()[0].getQuantity() == "1 lbs");
      REQUIRE(groceryList->getItems()[0].getSection() == StoreSection::Produce);
    }

    frame->Destroy();
  }
  wxUninitialize();
}

TEST_CASE("InputBar button enable/disable behavior") {
  wxInitialize();
  {
    auto frame =
        new wxFrame(nullptr, wxID_ANY, "InputBar button behavior tests");
    auto groceryList = std::make_unique<GroceryList>();
    auto *inputBar = new InputBar(frame, groceryList.get());

    SECTION("Button disabled on initialization") {
      REQUIRE_FALSE(inputBar->isAddButtonEnabledForTests());
    }

    SECTION("Button enabled when name, quantity, and section are valid") {
      inputBar->setNameForTests("banana");
      inputBar->setQuantityForTests("1 lbs");
      inputBar->setSectionForTests(1);

      inputBar->triggerUpdateButtonEventForTests();
      REQUIRE(inputBar->isAddButtonEnabledForTests());
    }

    SECTION("Button disabled when name is empty") {
      inputBar->setNameForTests("");
      inputBar->setQuantityForTests("1 lbs");
      inputBar->setSectionForTests(1);

      inputBar->triggerUpdateButtonEventForTests();
      REQUIRE_FALSE(inputBar->isAddButtonEnabledForTests());
    }
    SECTION("Button disabled when quantity is empty") {
      inputBar->setNameForTests("banana");
      inputBar->setQuantityForTests("");
      inputBar->setSectionForTests(1);

      inputBar->triggerUpdateButtonEventForTests();
      REQUIRE_FALSE(inputBar->isAddButtonEnabledForTests());
    }
    SECTION("Button disabled when section is zero") {
      inputBar->setNameForTests("banana");
      inputBar->setQuantityForTests("1 lbs");
      inputBar->setSectionForTests(0);

      inputBar->triggerUpdateButtonEventForTests();
      REQUIRE_FALSE(inputBar->isAddButtonEnabledForTests());
    }
    SECTION("Button disabled when all fields are empty") {
      inputBar->setNameForTests("");
      inputBar->setQuantityForTests("");
      inputBar->setSectionForTests(0);

      inputBar->triggerUpdateButtonEventForTests();
      REQUIRE_FALSE(inputBar->isAddButtonEnabledForTests());
    }

    SECTION("Button state updates on name change") {
      inputBar->setNameForTests("banana");
      inputBar->setQuantityForTests("1 lbs");
      inputBar->setSectionForTests(1);
      inputBar->triggerUpdateButtonEventForTests();
      REQUIRE(inputBar->isAddButtonEnabledForTests());

      inputBar->setNameForTests("");
      inputBar->triggerUpdateButtonEventForTests();
      REQUIRE_FALSE(inputBar->isAddButtonEnabledForTests());
    }

    SECTION("Button state updates on quantity change") {
      inputBar->setNameForTests("banana");
      inputBar->setQuantityForTests("1 lbs");
      inputBar->setSectionForTests(1);
      inputBar->triggerUpdateButtonEventForTests();
      REQUIRE(inputBar->isAddButtonEnabledForTests());

      inputBar->setQuantityForTests("");
      inputBar->triggerUpdateButtonEventForTests();
      REQUIRE_FALSE(inputBar->isAddButtonEnabledForTests());
    }

    SECTION("Button state updates on section change") {
      inputBar->setNameForTests("banana");
      inputBar->setQuantityForTests("1 lbs");
      inputBar->setSectionForTests(1);
      inputBar->triggerUpdateButtonEventForTests();
      REQUIRE(inputBar->isAddButtonEnabledForTests());

      inputBar->setSectionForTests(0);
      inputBar->triggerUpdateButtonEventForTests();
      REQUIRE_FALSE(inputBar->isAddButtonEnabledForTests());
    }

    frame->Destroy();
  }
  wxUninitialize();
}

TEST_CASE("InputBar interaction with GroceryList") {
  wxInitialize();
  {
    auto frame = new wxFrame(nullptr, wxID_ANY,
                             "Test InputBar interaction with GroceryList");
    auto groceryList = std::make_unique<GroceryList>();
    auto *inputBar = new InputBar(frame, groceryList.get());

    SECTION("Verifying the addition of multiple items") {
      inputBar->setNameForTests("Banana");
      inputBar->setQuantityForTests("3");
      inputBar->setSectionForTests(1); // Produce
      inputBar->clickAddForTests();

      inputBar->setNameForTests("Milk");
      inputBar->setQuantityForTests("1 gallon");
      inputBar->setSectionForTests(2); // Dairy
      inputBar->clickAddForTests();

      REQUIRE(groceryList->getItems().size() == 2);
      REQUIRE(groceryList->getItems()[0].getName() == "Banana");
      REQUIRE(groceryList->getItems()[1].getName() == "Milk");
    }

    SECTION("Items grouped by section correctly") {
      inputBar->setNameForTests("Banana");
      inputBar->setQuantityForTests("1 lbs");
      inputBar->setSectionForTests(1); // Produce
      inputBar->clickAddForTests();

      inputBar->setNameForTests("Apple");
      inputBar->setQuantityForTests("5 lbs");
      inputBar->setSectionForTests(1); // Produce
      inputBar->clickAddForTests();

      inputBar->setNameForTests("Milk");
      inputBar->setQuantityForTests("3 gallons");
      inputBar->setSectionForTests(2); // Dairy
      inputBar->clickAddForTests();

      auto produceItems = groceryList->getItemsBySection(StoreSection::Produce);
      auto dairyItems = groceryList->getItemsBySection(StoreSection::Dairy);

      REQUIRE(produceItems.size() == 2);
      REQUIRE(produceItems[0].getName() == "Banana");
      REQUIRE(produceItems[1].getName() == "Apple");

      REQUIRE(dairyItems.size() == 1);
      REQUIRE(dairyItems[0].getName() == "Milk");
    }

    SECTION("Free-Text quantity preserved") {
      inputBar->setNameForTests("Chicken");
      inputBar->setQuantityForTests("2 lbs");
      inputBar->setSectionForTests(3);

      inputBar->clickAddForTests();
      REQUIRE(groceryList->getItems()[0].getQuantity() == "2 lbs");
    }

    frame->Destroy();
  }
  wxUninitialize();
}

TEST_CASE("InputBar field clearing behavior") {
  auto frame =
      new wxFrame(nullptr, wxID_ANY, "InputBar field clearing behvaior test");
  auto groceryList = std::make_unique<GroceryList>();
  auto *inputBar = new InputBar(frame, groceryList.get());

  SECTION("Fields clear properly after item addition") {
    inputBar->setNameForTests("Banana");
    inputBar->setQuantityForTests("1 lbs");
    inputBar->setSectionForTests(1);
    inputBar->clickAddForTests();

    REQUIRE(inputBar->getNameForTests().IsEmpty());
    REQUIRE(inputBar->getQuantityForTests().IsEmpty());
    REQUIRE(inputBar->getSectionIndexForTests() == 0);
  }

  SECTION("Fields not cleared on validation failure") {
    inputBar->setNameForTests("");
    inputBar->setQuantityForTests("1 lbs");
    inputBar->setSectionForTests(1);

    inputBar->clickAddForTests();

    REQUIRE(inputBar->getNameForTests().IsEmpty());
    REQUIRE(inputBar->getQuantityForTests() == "1 lbs");
    REQUIRE(inputBar->getSectionIndexForTests() == 1);
  }
  frame->Destroy();
}

TEST_CASE("InputBar edge cases") {
  auto frame = new wxFrame(nullptr, wxID_ANY, "InputBar edge case tests");
  auto groceryList = std::make_unique<GroceryList>();
  auto inputBar = new InputBar(frame, groceryList.get());

  SECTION("Whitespace only name treated as empty input") {
    inputBar->setNameForTests(" ");
    inputBar->setQuantityForTests("1 lbs");
    inputBar->setSectionForTests(1);
    inputBar->clickAddForTests();

    REQUIRE(groceryList->getItems().empty());
  }

  SECTION("Whitespace only quantity treated as empty input") {
    inputBar->setNameForTests("Banana");
    inputBar->setQuantityForTests(" ");
    inputBar->setSectionForTests(1);
    inputBar->clickAddForTests();

    REQUIRE(groceryList->getItems().empty());
  }

  SECTION("Very long item name handled") {
    const std::string longName(500, 'a');
    inputBar->setNameForTests(longName);
    inputBar->setQuantityForTests("1 lbs");
    inputBar->setSectionForTests(1);

    inputBar->clickAddForTests();

    REQUIRE(groceryList->getItems().size() == 1);
    REQUIRE(groceryList->getItems()[0].getName() == longName);
  }

  SECTION("Special characters in item name handled") {
    inputBar->setNameForTests("Ben & Jerry's Ice Cream");
    inputBar->setQuantityForTests("1 pint");
    inputBar->setSectionForTests(4);

    inputBar->clickAddForTests();

    REQUIRE(groceryList->getItems().size() == 1);
    REQUIRE(groceryList->getItems()[0].getName() == "Ben & Jerry's Ice Cream");

  }
  frame->Destroy();
}

TEST_CASE("Observer notification test") {
 auto frame = new wxFrame(nullptr, wxID_ANY, "InputBar observer notification test");
  auto groceryList = std::make_unique<GroceryList>();
  auto *inputBar = new InputBar(frame, groceryList.get());

  class MockObserver : public IListObserver {
  public:
   int  callCount{};
    void onListChanged() override { callCount++; };
  };

  MockObserver mockObserver;
  groceryList->addObserver(&mockObserver);

  SECTION("Adding item triggers observer") {
    inputBar->setNameForTests("banana");
    inputBar->setQuantityForTests("1 lbs");
    inputBar->setSectionForTests(1);

    inputBar->clickAddForTests();

    REQUIRE(mockObserver.callCount == 1);
  }
}
