#include "employees/Administrator.h"

Administrator::Administrator(const std::string &id, const std::string &n)
    : Employee(id, n)
{
}

void Administrator::manageConsumablesStock(const IDisplay *display)
{
  if (display)
  {
    display->showLine("Administrator " + name + " managing consumables stock...");
  }
}

void Administrator::reviewConsumablesReports(const IDisplay *display)
{
  if (display)
  {
    display->showLine("Administrator " + name + " reviewing consumables reports...");
  }
}

std::string Administrator::getRole() const
{
  return "Administrator";
}
