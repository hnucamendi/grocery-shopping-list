#include "domain/StoreSection.h"
#include <stdexcept>
#include <string>

std::string toString(StoreSection ss) {
  switch (ss) {
  case StoreSection::Produce:
    return "produce";
  case StoreSection::Dairy:
    return "dairy";
  case StoreSection::Meat:
    return "meat";
  case StoreSection::Frozen:
    return "frozen";
  case StoreSection::Bakery:
    return "bakery";
  case StoreSection::Pantry:
    return "pantry";
  case StoreSection::Household:
    return "household";
  default:
    throw new std::invalid_argument("Invalid StoreSection Value");
  }
}
