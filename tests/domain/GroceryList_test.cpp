#include "domain/GroceryItem.h"
#include "domain/GroceryList.h"
#include "domain/StoreSection.h"
#include <catch2/catch_test_macros.hpp>
#include <regex>

TEST_CASE("Ensure GroceryList initializes properly") {
  GroceryItem banana("banana", "1.3 lbs", StoreSection::Produce);
  GroceryItem sandwhichMeat("deli ham", "1 lbs", StoreSection::Meat);

  std::regex pat("^\\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])$");
  GroceryList groceryList;

  groceryList.addItem(banana);
  groceryList.addItem(sandwhichMeat);

  REQUIRE(groceryList.getItems().size() == 2);

  REQUIRE(groceryList.getItems() ==
          std::vector<GroceryItem>{banana, sandwhichMeat});

  REQUIRE(groceryList.getItemsBySection(StoreSection::Produce) ==
          std::vector<GroceryItem>{banana});

  REQUIRE(groceryList.getItemsBySection(StoreSection::Meat) ==
          std::vector<GroceryItem>{sandwhichMeat});

  REQUIRE(std::regex_match(groceryList.getCreatedDate(), pat));
}
