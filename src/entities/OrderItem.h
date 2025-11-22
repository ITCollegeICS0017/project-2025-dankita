#ifndef ORDER_ITEM_H
#define ORDER_ITEM_H

#include <string>

class OrderItem
{
private:
  std::string itemID;
  int quantity;
  double unitPrice;

public:
  OrderItem(const std::string &id, int qty, double price);

  double getSubtotal() const;

  // Getters
  std::string getItemID() const;
  int getQuantity() const;
  double getUnitPrice() const;
};

#endif // ORDER_ITEM_H
