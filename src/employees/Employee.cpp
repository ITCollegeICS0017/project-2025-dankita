#include "employees/Employee.h"

Employee::Employee(const std::string &id, const std::string &n)
    : employeeID(id), name(n)
{
}

std::string Employee::getName() const
{
  return name;
}

std::string Employee::getID() const
{
  return employeeID;
}
