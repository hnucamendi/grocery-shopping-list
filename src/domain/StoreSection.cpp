#include "domain/StoreSection.h"
#include <cctype>
#include <stdexcept>
#include <string>
#include <vector>

std::string normalizePascal(const std::string &s) {
  if (s.empty()) {
    return s;
  }
  std::string result;
  result += std::toupper(s[0]);
  for (char c : s.substr(1)) {
    result += std::tolower(c);
  }
  return result;
}

std::string toString(StoreSection ss) {
  switch (ss) {
  case StoreSection::Produce:
    return "Produce";
  case StoreSection::Dairy:
    return "Dairy";
  case StoreSection::Meat:
    return "Meat";
  case StoreSection::Frozen:
    return "Frozen";
  case StoreSection::Bakery:
    return "Bakery";
  case StoreSection::Pantry:
    return "Pantry";
  case StoreSection::Household:
    return "Household";
  default:
    throw std::invalid_argument("Invalid StoreSection Value");
  }
}

StoreSection fromString(const std::string &s) {
  std::string ns = normalizePascal(s);
  if (ns == "Produce") {
    return StoreSection::Produce;
  } else if (ns == "Dairy") {
    return StoreSection::Dairy;
  } else if (ns == "Meat") {
    return StoreSection::Meat;
  } else if (ns == "Frozen") {
    return StoreSection::Frozen;
  } else if (ns == "Bakery") {
    return StoreSection::Bakery;
  } else if (ns == "Pantry") {
    return StoreSection::Pantry;
  } else if (ns == "Household") {
    return StoreSection::Household;
  } else {
    throw std::invalid_argument("Invalid StoreSection Value");
  }
}

std::vector<StoreSection> getAllSections() {
  return {
      StoreSection::Produce,   StoreSection::Dairy,  StoreSection::Meat,
      StoreSection::Frozen,    StoreSection::Bakery, StoreSection::Pantry,
      StoreSection::Household,
  };
}
