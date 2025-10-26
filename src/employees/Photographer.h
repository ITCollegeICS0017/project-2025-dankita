#ifndef PHOTOGRAPHER_H
#define PHOTOGRAPHER_H

#include "employees/Employee.h"
#include "entities/ConsumableUsage.h"
#include "interfaces/IDisplay.h"
#include <vector>
#include <string>

class Photographer : public Employee
{
public:
  Photographer(const std::string &id, const std::string &n);

  void viewAssignedOrders(const IDisplay *display);
  void submitConsumablesReport(const std::vector<ConsumableUsage> &items, const IDisplay *display);

  std::string getRole() const override;
};

#endif // PHOTOGRAPHER_H
