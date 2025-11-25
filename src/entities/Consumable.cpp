#include "entities/Consumable.h"
#include "exceptions/PhotoStudioExceptions.h"

Consumable::Consumable(const std::string &id, const std::string &n, int stock, const std::string &unit)
    : consumableID(id), name(n), currentStock(stock), unitOfMeasure(unit)
{
  // Data validation in constructor
  if (id.empty())
  {
    throw InvalidDataException(
        "Consumable ID cannot be empty",
        "Data validation failed in Consumable constructor");
  }

  if (n.empty())
  {
    throw InvalidDataException(
        "Consumable name cannot be empty",
        "Data validation failed in Consumable constructor");
  }

  if (stock < 0)
  {
    throw InvalidDataException(
        "Initial stock cannot be negative",
        "Data validation failed: stock < 0");
  }

  if (unit.empty())
  {
    throw InvalidDataException(
        "Unit of measure cannot be empty",
        "Data validation failed in Consumable constructor");
  }
}

void Consumable::updateStock(int quantity, const IDisplay *display)
{
  int newStock = currentStock + quantity;

  if (newStock < 0)
  {
    throw BusinessRuleException(
        "Cannot reduce stock below zero",
        "Stock update would result in negative stock: current=" +
            std::to_string(currentStock) + ", change=" + std::to_string(quantity));
  }

  currentStock = newStock;

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
