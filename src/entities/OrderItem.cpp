#include "entities/OrderItem.h"

OrderItem::OrderItem(const std::string &id, int qty, double price)
    : itemID(id), quantity(qty), unitPrice(price)
{
}

double OrderItem::getSubtotal() const
{
  return quantity * unitPrice;
}

std::string OrderItem::getItemID() const
{
  return itemID;
}

int OrderItem::getQuantity() const
{
  return quantity;
}

double OrderItem::getUnitPrice() const
{
  return unitPrice;
}
