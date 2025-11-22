#ifndef ORDER_MANAGER_H
#define ORDER_MANAGER_H

#include <vector>
#include <memory>
#include "orders/Order.h"
#include "entities/Client.h"
#include "entities/Service.h"
#include "interfaces/IDisplay.h"
#include "config/Config.h"
#include "exceptions/PhotoStudioExceptions.h"

class OrderManager
{
private:
  std::vector<Order *> orders;
  const IDisplay *display;
  const Config *config;

public:
  OrderManager(const IDisplay *disp, const Config *cfg);
  ~OrderManager();

  Order *createOrder(const std::string &orderID, Client *client, const std::string &completionTime, bool isExpress);
  void addItemToOrder(Order *order, const std::string &itemID, int quantity, double unitPrice);
  void processOrder(Order *order);
  void completeOrder(Order *order);
  void recordPayment(Order *order);

  const std::vector<Order *> &getAllOrders() const;
  double calculateTotalRevenue() const;

private:
  void validateOrderCreation(const std::string &orderID, Client *client, const std::string &completionTime) const;
  void validateOrderItem(const std::string &itemID, int quantity, double unitPrice) const;
  void validateOrderExists(Order *order) const;
  void validateOrderStatus(Order *order, OrderStatus expectedStatus) const;
  bool isOrderIDDuplicate(const std::string &orderID) const;
};

#endif // ORDER_MANAGER_H
