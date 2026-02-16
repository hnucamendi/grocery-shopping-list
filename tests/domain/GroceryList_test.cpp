#include "domain/GroceryItem.h"
#include "domain/GroceryList.h"
#include "domain/StoreSection.h"
#include <catch2/catch_test_macros.hpp>
#include <regex>

class MockObserver : public IListObserver {
public:
  int callCount = 0;
  void onListChanged() override { this->callCount++; }
};

TEST_CASE("Ensure GroceryList initializes properly") {
  GroceryItem banana("banana", "1.3 lbs", StoreSection::Produce);
  GroceryItem sandwichMeat("deli ham", "1 lbs", StoreSection::Meat);

  std::regex pat("^\\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])$");
  GroceryList groceryList;

  groceryList.addItem(banana);
  groceryList.addItem(sandwichMeat);

  REQUIRE(groceryList.getItems().size() == 2);

  REQUIRE(groceryList.getItems() ==
          std::vector<GroceryItem>{banana, sandwichMeat});

  REQUIRE(groceryList.getItemsBySection(StoreSection::Produce) ==
          std::vector<GroceryItem>{banana});

  REQUIRE(groceryList.getItemsBySection(StoreSection::Meat) ==
          std::vector<GroceryItem>{sandwichMeat});

  REQUIRE(std::regex_match(groceryList.getCreatedDate(), pat));
}

TEST_CASE("Test Observer Logic") {
  GroceryItem banana("banana", "1.3 lbs", StoreSection::Produce);
  GroceryList groceryList;

  SECTION("observer receives notification when item is added") {
    MockObserver observer;
    groceryList.addObserver(&observer);

    REQUIRE(observer.callCount == 0);
    groceryList.addItem(banana);
    REQUIRE(observer.callCount == 1);
  }

  SECTION("multiple observers all get notified") {
    MockObserver o1;
    MockObserver o2;
    groceryList.addObserver(&o1);
    groceryList.addObserver(&o2);

    REQUIRE(o1.callCount == 0);
    REQUIRE(o2.callCount == 0);
    groceryList.addItem(banana);
    REQUIRE(o1.callCount == 1);
    REQUIRE(o2.callCount == 1);
  }

  SECTION("duplicate observer only gets notified once") {
    MockObserver observer;
    groceryList.addObserver(&observer);
    groceryList.addObserver(&observer);

    REQUIRE(observer.callCount == 0);
    groceryList.addItem(banana);
    REQUIRE(observer.callCount == 1);
  }
  SECTION("removed observer does not get notified") {
    MockObserver observer;
    groceryList.addObserver(&observer);
    groceryList.removeObserver(&observer);

    REQUIRE(observer.callCount == 0);
    groceryList.addItem(banana);
    REQUIRE(observer.callCount == 0);
  }

  SECTION("Nullptr observer is safely ignored") {
    MockObserver *observer = nullptr;
    groceryList.addObserver(observer);

    groceryList.addItem(banana);
    REQUIRE(groceryList.getItems().size() == 1);
  }
}
