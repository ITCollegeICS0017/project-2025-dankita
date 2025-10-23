#include "managers/OrderManager.h"
#include "orders/ExpressOrder.h"

OrderManager::OrderManager(const IDisplay *disp, const Config *cfg)
    : display(disp), config(cfg)
{
}

OrderManager::~OrderManager()
{
  // Clean up orders (if ownership is with the manager)
}

Order *OrderManager::createOrder(const std::string &orderID, Client *client,
                                 const std::string &completionTime, bool isExpress)
{
  Order *order = nullptr;
  if (isExpress)
  {
    order = new ExpressOrder(orderID, completionTime, client, config);
  }
  else
  {
    order = new Order(orderID, completionTime, client);
  }

  orders.push_back(order);

  if (display)
  {
    std::string msg = "Order " + orderID + " created for client " + client->getSurname();
    if (isExpress)
    {
      msg += " (EXPRESS)";
    }
    display->showLine(msg);
  }

  return order;
}

void OrderManager::addItemToOrder(Order *order, const std::string &itemID, int quantity, double unitPrice)
{
  OrderItem *item = new OrderItem(itemID, quantity, unitPrice);
  order->addItem(item);
}

void OrderManager::processOrder(Order *order)
{
  order->updateStatus(OrderStatus::IN_PROGRESS, display);
}

void OrderManager::completeOrder(Order *order)
{
  order->updateStatus(OrderStatus::COMPLETED, display);

  double price = order->calculatePrice();

  if (display)
  {
    display->showLine("Order " + order->getOrderID() + " total price: $" + std::to_string(price));
  }
}

void OrderManager::recordPayment(Order *order)
{
  order->recordPayment(display);
}

const std::vector<Order *> &OrderManager::getAllOrders() const
{
  return orders;
}

double OrderManager::calculateTotalRevenue() const
{
  double total = 0.0;
  for (const auto &order : orders)
  {
    if (order->getIsPaid())
    {
      total += order->getTotalPrice();
    }
  }
  return total;
}
