#include "orders/Order.h"

Order::Order(const std::string &id, const std::string &cTime, Client *c)
    : orderID(id), completionTime(cTime), status(OrderStatus::PENDING),
      totalPrice(0.0), isPaid(false), client(c)
{
}

double Order::calculatePrice()
{
  if (items.empty())
  {
    return totalPrice;
  }

  totalPrice = 0.0;
  for (const auto &item : items)
  {
    totalPrice += item->getSubtotal();
  }

  return totalPrice;
}

void Order::updateStatus(OrderStatus newStatus, const IDisplay *display)
{
  status = newStatus;

  if (display)
  {
    std::string statusStr;
    switch (status)
    {
    case OrderStatus::PENDING:
      statusStr = "PENDING";
      break;
    case OrderStatus::IN_PROGRESS:
      statusStr = "IN_PROGRESS";
      break;
    case OrderStatus::COMPLETED:
      statusStr = "COMPLETED";
      break;
    case OrderStatus::CANCELLED:
      statusStr = "CANCELLED";
      break;
    }
    display->showLine("Order " + orderID + " status updated to " + statusStr);
  }
}

void Order::recordPayment(const IDisplay *display)
{
  isPaid = true;

  if (display)
  {
    display->showLine("Payment recorded for order " + orderID);
  }
}

void Order::addItem(OrderItem *item)
{
  items.push_back(item);
  totalPrice += item->getSubtotal();
}

std::string Order::getOrderID() const
{
  return orderID;
}

std::string Order::getCompletionTime() const
{
  return completionTime;
}

OrderStatus Order::getStatus() const
{
  return status;
}

double Order::getTotalPrice() const
{
  return totalPrice;
}

bool Order::getIsPaid() const
{
  return isPaid;
}

Client *Order::getClient() const
{
  return client;
}

const std::vector<OrderItem *> &Order::getItems() const
{
  return items;
}
