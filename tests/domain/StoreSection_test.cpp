#include "domain/StoreSection.h"
#include <catch2/catch_test_macros.hpp>
#include <stdexcept>
#include <string>
#include <vector>

TEST_CASE("Normalize pascal string") {
  struct TestFields {
    std::string given;
    std::string expected;
  };

  std::vector<TestFields> fields = {
      {"micuit", "Micuit"},
      {"Orange", "Orange"},
      {"eLArIs", "Elaris"},
      {"MONEY", "Money"},
      {"sTIcKeR", "Sticker"},
      {"", ""},
      {"sTIcKeR Bomb", "Sticker bomb"},
  };

  for (TestFields &f : fields) {
    REQUIRE(normalizePascal(f.given) == f.expected);
  }
}

TEST_CASE("StoreSection toString conversion") {
  SECTION("happy path tests") {
    struct TestFields {
      StoreSection given;
      std::string expected;
    };

    std::vector<TestFields> fields = {
        {StoreSection::Produce, "Produce"},
        {StoreSection::Bakery, "Bakery"},
        {StoreSection::Frozen, "Frozen"},
        {StoreSection::Dairy, "Dairy"},
        {StoreSection::Household, "Household"},
        {StoreSection::Meat, "Meat"},
        {StoreSection::Pantry, "Pantry"},
    };

    for (TestFields &f : fields) {
      REQUIRE(toString(f.given) == f.expected);
    }
  }

  SECTION("expect exception from invalid input") {
    REQUIRE_THROWS(toString(static_cast<StoreSection>(9999)));
  }
}

TEST_CASE("StoreSection fromString conversion") {
  SECTION("expect exception from invalid input") {
    REQUIRE_THROWS_AS(fromString("Cats!"), std::invalid_argument);
  }

  SECTION("happy path tests") {
    struct TestFields {
      std::string given;
      StoreSection expected;
    };

    std::vector<TestFields> fields = {
        {"Produce", StoreSection::Produce},
        {"produce", StoreSection::Produce},
        {"PRODUCE", StoreSection::Produce},
        {"prOdUcE", StoreSection::Produce},
        {"Bakery", StoreSection::Bakery},
        {"bakery", StoreSection::Bakery},
        {"BAKERY", StoreSection::Bakery},
        {"baKeRy", StoreSection::Bakery},
        {"Frozen", StoreSection::Frozen},
        {"frozen", StoreSection::Frozen},
        {"FROZEN", StoreSection::Frozen},
        {"frOzEn", StoreSection::Frozen},
        {"Dairy", StoreSection::Dairy},
        {"dairy", StoreSection::Dairy},
        {"DAIRY", StoreSection::Dairy},
        {"DaIrY", StoreSection::Dairy},
        {"Household", StoreSection::Household},
        {"household", StoreSection::Household},
        {"HOUSEHOLD", StoreSection::Household},
        {"hoUsEhOlD", StoreSection::Household},
        {"Meat", StoreSection::Meat},
        {"meat", StoreSection::Meat},
        {"MEAT", StoreSection::Meat},
        {"mEaT", StoreSection::Meat},
        {"Pantry", StoreSection::Pantry},
        {"pantry", StoreSection::Pantry},
        {"PANTRY", StoreSection::Pantry},
        {"pAnTrY", StoreSection::Pantry},
    };

    for (TestFields &f : fields) {
      REQUIRE(fromString(f.given) == f.expected);
    }
  }
}

TEST_CASE("fetch all StoreSection enums") {
  std::vector<StoreSection> expectedSections = {
      StoreSection::Produce,   StoreSection::Dairy,  StoreSection::Meat,
      StoreSection::Frozen,    StoreSection::Bakery, StoreSection::Pantry,
      StoreSection::Household,
  };

  REQUIRE(getAllSections() == expectedSections);
}
