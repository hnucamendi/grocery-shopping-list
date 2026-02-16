#ifndef GROCERY_SHOPPING_LIST_DOMAIN_GROCERYLIST_H
#define GROCERY_SHOPPING_LIST_DOMAIN_GROCERYLIST_H

#include "domain/GroceryItem.h"
#include "domain/IListObserver.h"
#include <string>
#include <vector>

class GroceryList {
public:
  GroceryList();
  void addItem(const GroceryItem &item);
  const std::vector<GroceryItem> &getItems() const;
  std::vector<GroceryItem> getItemsBySection(const StoreSection section) const;
  std::string getCreatedDate() const;
  void addObserver(IListObserver *);
  void removeObserver(IListObserver *);

private:
  std::vector<IListObserver *> m_observers;
  std::vector<GroceryItem> m_items;
  std::string m_createdDate;
  static std::string getCurrentDateString();
  void notifyObservers();
};

#endif
