#include "domain/GroceryList.h"
#include <algorithm>
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

void GroceryList::addItem(const GroceryItem &item) {
  m_items.push_back(item);
  notifyObservers();
}

const std::vector<GroceryItem> &GroceryList::getItems() const {
  return m_items;
}

std::vector<GroceryItem>
GroceryList::getItemsBySection(const StoreSection section) const {
  std::vector<GroceryItem> filteredVector;
  for (const auto &item : m_items) {
    if (item.getSection() == section) {
      filteredVector.push_back(item);
    }
  }
  return filteredVector;
}

void GroceryList::addObserver(IListObserver *observer) {
  auto it = std::find(m_observers.begin(), m_observers.end(), observer);
  if (it == m_observers.end()) {
    m_observers.push_back(observer);
  }
}

void GroceryList::removeObserver(IListObserver *observer) {
  m_observers.erase(
      std::remove(m_observers.begin(), m_observers.end(), observer),
      m_observers.end());
}

void GroceryList::notifyObservers() {
  for (auto *observer : m_observers) {
    observer->onListChanged();
  }
}
