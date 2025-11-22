#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "employees/Employee.h"
#include "interfaces/IDisplay.h"
#include <string>

class Administrator : public Employee
{
public:
  Administrator(const std::string &id, const std::string &n);

  void manageConsumablesStock(const IDisplay *display);
  void reviewConsumablesReports(const IDisplay *display);

  std::string getRole() const override;
};

#endif // ADMINISTRATOR_H
