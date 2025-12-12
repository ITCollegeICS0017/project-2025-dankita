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
#include "repository/OrderRepository.h"
#include "repository/OrderRecord.h"
#include "repository/FileManager.h"


class OrderManager
{
private:
  std::vector<Order *> orders;   // In-memory working orders
  std::vector<Client *> clients; // In-memory clients (created from loaded records)
  const IDisplay *display;
  const Config *config;

  // Release 4: Repository integration
  OrderRepository *repository;
  FileManager *fileManager;

public:
  OrderManager(const IDisplay *disp, const Config *cfg);
  ~OrderManager();

  // Release 4: Repository connection methods (Step 8)
  void setRepository(OrderRepository *repo);
  void setFileManager(FileManager *fm);
  OrderRepository *getRepository();

  // Release 4: Data persistence methods
  void loadData();                                                 // Load from file and create entities
  void saveData();                                                 // Save all entities to file
  void syncOrderToRepository(Order *order);                        // Sync single order to repository
  OrderRecord createRecordFromOrder(Order *order, Client *client); // Create record from Order

  Order *createOrder(const std::string &orderID, Client *client, const std::string &completionTime, bool isExpress);
  void addItemToOrder(Order *order, const std::string &itemID, int quantity, double unitPrice);
  void processOrder(Order *order);
  void completeOrder(Order *order);
  void recordPayment(Order *order);

  // Release 4: Methods to work with loaded entities
  Order *findOrderById(const std::string &orderID);
  Client *findOrCreateClient(const std::string &clientID, const std::string &surname);
  int getLoadedOrderCount() const;

  const std::vector<Order *> &getAllOrders() const;
  const std::vector<Client *> &getAllClients() const;
  double calculateTotalRevenue() const;

private:
  void validateOrderCreation(const std::string &orderID, Client *client, const std::string &completionTime) const;
  void validateOrderItem(const std::string &itemID, int quantity, double unitPrice) const;
  void validateOrderExists(Order *order) const;
  void validateOrderStatus(Order *order, OrderStatus expectedStatus) const;
  bool isOrderIDDuplicate(const std::string &orderID) const;

  int statusToInt(OrderStatus status) const;
  OrderStatus intToStatus(int status) const;

  Order *createOrderFromRecord(const OrderRecord &record, Client *client);
};

#endif // ORDER_MANAGER_H
