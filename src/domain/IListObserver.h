#ifndef GROCERY_SHOPPING_LIST_DOMAIN_ILISTOBSERVER_H
#define GROCERY_SHOPPING_LIST_DOMAIN_ILISTOBSERVER_H

class IListObserver {
public:
  virtual ~IListObserver() = default;
  virtual void onListChanged() = 0;
};

#endif
