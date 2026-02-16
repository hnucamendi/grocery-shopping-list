#include "domain/GroceryItem.h"
#include "domain/StoreSection.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Ensure GroceryItem initializes properly") {
  GroceryItem groceryItem("bananas", "1.3 lbs", StoreSection::Produce);
  REQUIRE(groceryItem.getName() == "bananas");
  REQUIRE(groceryItem.getQuantity() == "1.3 lbs");
  REQUIRE(groceryItem.getSection() == StoreSection::Produce);
}
