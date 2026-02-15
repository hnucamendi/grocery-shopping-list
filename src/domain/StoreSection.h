#ifndef GROCERY_SHOPPING_LIST_DOMAIN_STORESECTION_H
#define GROCERY_SHOPPING_LIST_DOMAIN_STORESECTION_H

#include <string>
#include <vector>

enum class StoreSection {
  Produce,
  Dairy,
  Meat,
  Frozen,
  Bakery,
  Pantry,
  Household
};

std::string normalizePascal(const std::string &s);
std::string toString(StoreSection ss);
StoreSection fromString(const std::string &s);
std::vector<StoreSection> getAllSections();

#endif
