#include "entities/Consumable.h"

Consumable::Consumable(const std::string &id, const std::string &n, int stock, const std::string &unit)
    : consumableID(id), name(n), currentStock(stock), unitOfMeasure(unit)
{
}

void Consumable::updateStock(int quantity, const IDisplay *display)
{
  currentStock += quantity;

  if (display)
  {
    display->showLine("Stock updated for " + name + ": " + std::to_string(currentStock) + " " + unitOfMeasure);
  }
}

int Consumable::getCurrentStock() const
{
  return currentStock;
}

std::string Consumable::getName() const
{
  return name;
}

std::string Consumable::getConsumableID() const
{
  return consumableID;
}

std::string Consumable::getUnitOfMeasure() const
{
  return unitOfMeasure;
}
