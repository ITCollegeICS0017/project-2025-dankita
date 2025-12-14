#include "employees/Photographer.h"

Photographer::Photographer(const std::string &id, const std::string &n)
    : Employee(id, n)
{
}

void Photographer::viewAssignedOrders(const IDisplay *display)
{
  if (display)
  {
    display->showLine("Photographer " + name + " viewing assigned orders...");
  }
}

void Photographer::submitConsumablesReport(const std::vector<ConsumableUsage> &items, const IDisplay *display)
{
  if (display)
  {
    display->showLine("Photographer " + name + " submitting consumables report...");
  }
}

std::string Photographer::getRole() const
{
  return "Photographer";
}
