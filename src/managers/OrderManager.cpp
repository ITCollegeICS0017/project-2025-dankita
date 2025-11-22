#include "managers/OrderManager.h"
#include "orders/ExpressOrder.h"
#include "exceptions/PhotoStudioExceptions.h"
#include <algorithm>

OrderManager::OrderManager(const IDisplay *disp, const Config *cfg)
    : display(disp), config(cfg)
{
}

OrderManager::~OrderManager()
{
  // Clean up orders (if ownership is manager)
}

Order *OrderManager::createOrder(const std::string &orderID, Client *client,
                                 const std::string &completionTime, bool isExpress)
{
  validateOrderCreation(orderID, client, completionTime);

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

  if (order->getStatus() != OrderStatus::PENDING)
  {
    throw ValidationException(
        "Order creation failed - invalid initial state",
        "Postcondition violation: Order status should be PENDING after creation");
  }

  return order;
}

void OrderManager::addItemToOrder(Order *order, const std::string &itemID, int quantity, double unitPrice)
{
  validateOrderExists(order);
  validateOrderItem(itemID, quantity, unitPrice);

  OrderItem *item = new OrderItem(itemID, quantity, unitPrice);
  order->addItem(item);
}

void OrderManager::processOrder(Order *order)
{
  validateOrderExists(order);
  validateOrderStatus(order, OrderStatus::PENDING);

  order->updateStatus(OrderStatus::IN_PROGRESS, display);

  if (order->getStatus() != OrderStatus::IN_PROGRESS)
  {
    throw ValidationException(
        "Order processing failed",
        "Postcondition violation: Order status should be IN_PROGRESS");
  }
}

void OrderManager::completeOrder(Order *order)
{
  validateOrderExists(order);
  validateOrderStatus(order, OrderStatus::IN_PROGRESS);

  order->updateStatus(OrderStatus::COMPLETED, display);

  double price = order->calculatePrice();

  if (display)
  {
    display->showLine("Order " + order->getOrderID() + " total price: $" + std::to_string(price));
  }

  // Postcondition: order status should be COMPLETED and price > 0
  if (order->getStatus() != OrderStatus::COMPLETED)
  {
    throw ValidationException(
        "Order completion failed",
        "Postcondition violation: Order status should be COMPLETED");
  }

  if (price < 0)
  {
    throw ValidationException(
        "Order completion failed - invalid price",
        "Postcondition violation: Order price cannot be negative");
  }
}

void OrderManager::recordPayment(Order *order)
{
  // Precondition: order must exist and be COMPLETED
  validateOrderExists(order);
  validateOrderStatus(order, OrderStatus::COMPLETED);

  // Precondition: order must have items and a valid price
  if (order->getItems().empty())
  {
    throw BusinessRuleException(
        "Cannot record payment - order has no items",
        "Business rule violation: Payment requires at least one order item");
  }

  if (order->getTotalPrice() <= 0)
  {
    throw BusinessRuleException(
        "Cannot record payment - invalid order amount",
        "Business rule violation: Payment amount must be positive");
  }

  order->recordPayment(display);

  if (!order->getIsPaid())
  {
    throw ValidationException(
        "Payment recording failed",
        "Postcondition violation: Order should be marked as paid");
  }
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

void OrderManager::validateOrderCreation(const std::string &orderID, Client *client,
                                         const std::string &completionTime) const
{
  if (orderID.empty())
  {
    throw InvalidDataException(
        "Order ID cannot be empty",
        "Precondition violation: orderID.empty()");
  }

  if (isOrderIDDuplicate(orderID))
  {
    throw DuplicateDataException(
        "Order ID already exists: " + orderID,
        "Precondition violation: duplicate orderID");
  }

  if (client == nullptr)
  {
    throw InvalidDataException(
        "Client information is required",
        "Precondition violation: client == nullptr");
  }

  if (completionTime.empty())
  {
    throw InvalidDataException(
        "Completion time is required",
        "Precondition violation: completionTime.empty()");
  }
}

void OrderManager::validateOrderItem(const std::string &itemID, int quantity, double unitPrice) const
{
  if (itemID.empty())
  {
    throw InvalidDataException(
        "Item ID cannot be empty",
        "Precondition violation: itemID.empty()");
  }

  if (quantity <= 0)
  {
    throw InvalidDataException(
        "Quantity must be greater than zero",
        "Precondition violation: quantity <= 0");
  }

  if (unitPrice < 0)
  {
    throw InvalidDataException(
        "Unit price cannot be negative",
        "Precondition violation: unitPrice < 0");
  }
}

void OrderManager::validateOrderExists(Order *order) const
{
  // Precondition: order pointer must not be null
  if (order == nullptr)
  {
    throw DataNotFoundException(
        "Order not found",
        "Precondition violation: order == nullptr");
  }
}

void OrderManager::validateOrderStatus(Order *order, OrderStatus expectedStatus) const
{
  if (order->getStatus() != expectedStatus)
  {
    std::string expectedStr;
    switch (expectedStatus)
    {
    case OrderStatus::PENDING:
      expectedStr = "PENDING";
      break;
    case OrderStatus::IN_PROGRESS:
      expectedStr = "IN_PROGRESS";
      break;
    case OrderStatus::COMPLETED:
      expectedStr = "COMPLETED";
      break;
    case OrderStatus::CANCELLED:
      expectedStr = "CANCELLED";
      break;
    }

    throw BusinessRuleException(
        "Invalid order status - expected " + expectedStr,
        "Precondition violation: order status mismatch");
  }
}

bool OrderManager::isOrderIDDuplicate(const std::string &orderID) const
{
  for (const auto &order : orders)
  {
    if (order->getOrderID() == orderID)
    {
      return true;
    }
  }
  return false;
}
