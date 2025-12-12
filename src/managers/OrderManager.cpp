#include "managers/OrderManager.h"
#include "orders/ExpressOrder.h"
#include "exceptions/PhotoStudioExceptions.h"
#include <algorithm>

OrderManager::OrderManager(const IDisplay *disp, const Config *cfg)
    : display(disp), config(cfg), repository(nullptr), fileManager(nullptr)
{
}

OrderManager::~OrderManager()
{
  // Clean up orders
  for (auto *order : orders)
  {
    delete order;
  }
  orders.clear();

  for (auto *client : clients)
  {
    delete client;
  }
  clients.clear();
}

void OrderManager::setRepository(OrderRepository *repo)
{
  repository = repo;
}

void OrderManager::setFileManager(FileManager *fm)
{
  fileManager = fm;
}

OrderRepository *OrderManager::getRepository()
{
  return repository;
}

int OrderManager::statusToInt(OrderStatus status) const
{
  switch (status)
  {
  case OrderStatus::PENDING:
    return 0;
  case OrderStatus::IN_PROGRESS:
    return 1;
  case OrderStatus::COMPLETED:
    return 2;
  case OrderStatus::CANCELLED:
    return 3;
  default:
    return 0;
  }
}

OrderStatus OrderManager::intToStatus(int status) const
{
  switch (status)
  {
  case 0:
    return OrderStatus::PENDING;
  case 1:
    return OrderStatus::IN_PROGRESS;
  case 2:
    return OrderStatus::COMPLETED;
  case 3:
    return OrderStatus::CANCELLED;
  default:
    return OrderStatus::PENDING;
  }
}

Client *OrderManager::findOrCreateClient(const std::string &clientID, const std::string &surname)
{
  for (auto *client : clients)
  {
    if (client->getID() == clientID)
    {
      return client;
    }
  }

  Client *newClient = new Client(clientID, surname);
  clients.push_back(newClient);
  return newClient;
}

Order *OrderManager::findOrderById(const std::string &orderID)
{
  for (auto *order : orders)
  {
    if (order->getOrderID() == orderID)
    {
      return order;
    }
  }
  return nullptr;
}

int OrderManager::getLoadedOrderCount() const
{
  return static_cast<int>(orders.size());
}

const std::vector<Client *> &OrderManager::getAllClients() const
{
  return clients;
}

Order *OrderManager::createOrderFromRecord(const OrderRecord &record, Client *client)
{
  Order *order = nullptr;

  if (record.isExpress)
  {
    order = new ExpressOrder(record.orderID, record.completionTime, client, config);
  }
  else
  {
    order = new Order(record.orderID, record.completionTime, client);
  }

  order->restoreStatus(intToStatus(record.status));
  order->restorePrice(record.totalPrice);
  order->restorePaidStatus(record.isPaid);

  return order;
}

OrderRecord OrderManager::createRecordFromOrder(Order *order, Client *client)
{
  OrderRecord record;
  record.orderID = order->getOrderID();
  record.clientID = client ? client->getID() : "";
  record.clientSurname = client ? client->getSurname() : "";
  record.completionTime = order->getCompletionTime();

  ExpressOrder *expressOrder = dynamic_cast<ExpressOrder *>(order);
  record.isExpress = (expressOrder != nullptr);

  record.status = statusToInt(order->getStatus());
  record.totalPrice = order->getTotalPrice();
  record.isPaid = order->getIsPaid();

  return record;
}

void OrderManager::syncOrderToRepository(Order *order)
{
  if (!repository || !order)
    return;

  Client *client = order->getClient();
  OrderRecord record = createRecordFromOrder(order, client);

  int index = repository->findIndexById(order->getOrderID());
  if (index >= 0)
  {
    // Update existing record
    repository->updateAt(index, record);
  }
  else
  {
    // Add new record
    repository->add(record);
  }
}

/**
 * LoadData - Load from file and create actual working entities
 *
 * 1. Loads records from file into repository
 * 2. Creates actual Order and Client objects from those records
 * 3. Adds them to the working collections so they can be interacted with
 */
void OrderManager::loadData()
{
  if (!repository || !fileManager)
    return;

  // Step 1: Load from file into repository
  fileManager->loadFromFile(*repository);

  // Step 2: Create actual Order and Client objects from loaded records
  for (int i = 0; i < repository->getCount(); i++)
  {
    const OrderRecord &record = repository->getAt(i);

    // Find or create the client for this order
    Client *client = findOrCreateClient(record.clientID, record.clientSurname);

    // Create the Order object with restored state
    Order *order = createOrderFromRecord(record, client);

    // Add to our working collection
    orders.push_back(order);
  }

  if (display && !orders.empty())
  {
    display->showLine("Loaded " + std::to_string(orders.size()) + " order(s) as working entities.");
  }
}

void OrderManager::saveData()
{
  if (!repository || !fileManager)
    return;

  repository->clear();

  for (auto *order : orders)
  {
    syncOrderToRepository(order);
  }

  if (repository->getCount() > 0)
  {
    fileManager->saveToFile(*repository);
  }
  else if (display)
  {
    display->showLine("No orders to save.");
  }
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

  // Sync to repository
  syncOrderToRepository(order);

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

  syncOrderToRepository(order);
}

void OrderManager::processOrder(Order *order)
{
  validateOrderExists(order);
  validateOrderStatus(order, OrderStatus::PENDING);

  order->updateStatus(OrderStatus::IN_PROGRESS, display);

  syncOrderToRepository(order);

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

  syncOrderToRepository(order);

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

  // Precondition: order must have a valid price (items may not be stored for loaded orders)
  if (order->getTotalPrice() <= 0)
  {
    throw BusinessRuleException(
        "Cannot record payment - invalid order amount",
        "Business rule violation: Payment amount must be positive");
  }

  order->recordPayment(display);

  syncOrderToRepository(order);

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
