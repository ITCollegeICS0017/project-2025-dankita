#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include "types/Types.h"
#include "entities/Client.h"
#include "entities/OrderItem.h"
#include "interfaces/IDisplay.h"
#include "exceptions/PhotoStudioExceptions.h"

class Order
{
private:
  std::string orderID;
  std::string completionTime;
  OrderStatus status;
  double totalPrice;
  bool isPaid;
  Client *client;
  std::vector<OrderItem *> items;

public:
  Order(const std::string &id, const std::string &cTime, Client *c);
  virtual ~Order() = default;

  // Virtual function for polymorphism
  virtual double calculatePrice();

  void updateStatus(OrderStatus newStatus, const IDisplay *display);
  void recordPayment(const IDisplay *display);
  void addItem(OrderItem *item);

  // Release 4: Methods for restoring state from persistent storage
  void restoreStatus(OrderStatus restoredStatus);
  void restorePrice(double restoredPrice);
  void restorePaidStatus(bool paid);

  std::string getOrderID() const;
  std::string getCompletionTime() const;
  OrderStatus getStatus() const;
  double getTotalPrice() const;
  bool getIsPaid() const;
  Client *getClient() const;
  const std::vector<OrderItem *> &getItems() const;
};

#endif // ORDER_H
