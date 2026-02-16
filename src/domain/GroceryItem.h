#ifndef GROCERY_SHOPPING_LIST_DOMAIN_GROCERYITEM_H
#define GROCERY_SHOPPING_LIST_DOMAIN_GROCERYITEM_H

#include "domain/StoreSection.h"
#include <string>

class GroceryItem {
public:
  bool operator==(const GroceryItem &other) const;

  GroceryItem(const std::string &name, const std::string &quantity,
              StoreSection section);

  std::string getName() const;
  std::string getQuantity() const;
  StoreSection getSection() const;

private:
  std::string m_name;
  std::string m_quantity;
  StoreSection m_section;
};

#endif
