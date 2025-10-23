#ifndef RECEPTIONIST_H
#define RECEPTIONIST_H

#include "employees/Employee.h"
#include "entities/Client.h"
#include "interfaces/IDisplay.h"
#include <string>

class Order;

class Receptionist : public Employee
{
public:
  Receptionist(const std::string &id, const std::string &n);

  void createOrder(Client *client, Order *order, const IDisplay *display);
  void generateDailyRevenueReport(const IDisplay *display);

  std::string getRole() const override;
};

#endif // RECEPTIONIST_H
