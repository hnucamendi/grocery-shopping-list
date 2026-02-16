#include "domain/GroceryList.h"
#include "domain/GroceryItem.h"
#include "domain/StoreSection.h"
#include <ctime>
#include <iomanip>
#include <sstream>

std::string GroceryList::getCurrentDateString() {
  std::time_t now = std::time(nullptr);
  std::tm tm = *std::localtime(&now);

  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y-%m-%d");
  return oss.str();
}

GroceryList::GroceryList()
    : m_createdDate(GroceryList::getCurrentDateString()) {}

std::string GroceryList::getCreatedDate() const { return m_createdDate; }

void GroceryList::addItem(const GroceryItem &item) { m_items.push_back(item); }

std::vector<GroceryItem> GroceryList::getItems() const { return m_items; }

std::vector<GroceryItem>
GroceryList::getItemsBySection(const StoreSection &section) const {
  std::vector<GroceryItem> filteredVector;
  for (std::size_t i = 0; i < m_items.size(); i++) {
    if (m_items[i].getSection() == section) {
      filteredVector.push_back(m_items[i]);
    }
  }
  return filteredVector;
}
