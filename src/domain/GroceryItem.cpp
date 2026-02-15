#include "domain/GroceryItem.h"

GroceryItem::GroceryItem(const std::string &name, const std::string &quantity,
                         StoreSection section)
    : m_name(name), m_quantity(quantity), m_section(section) {};

std::string GroceryItem::getName() const { return m_name; }
std::string GroceryItem::getQuantity() const { return m_quantity; }
StoreSection GroceryItem::getSection() const { return m_section; }
