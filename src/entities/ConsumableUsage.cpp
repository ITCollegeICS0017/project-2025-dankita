#include "entities/ConsumableUsage.h"
#include "exceptions/PhotoStudioExceptions.h"

ConsumableUsage::ConsumableUsage(const std::string &id, const std::string &name, int qty)
    : usageID(id), consumableName(name), quantityUsed(qty)
{
  // Data validation in constructor
  if (id.empty())
  {
    throw InvalidDataException(
        "Usage ID cannot be empty",
        "Data validation failed in ConsumableUsage constructor");
  }

  if (name.empty())
  {
    throw InvalidDataException(
        "Consumable name cannot be empty",
        "Data validation failed in ConsumableUsage constructor");
  }

  if (qty <= 0)
  {
    throw InvalidDataException(
        "Usage quantity must be positive",
        "Data validation failed: quantity <= 0");
  }
}

std::string ConsumableUsage::getUsageID() const
{
  return usageID;
}

std::string ConsumableUsage::getConsumableName() const
{
  return consumableName;
}

int ConsumableUsage::getQuantityUsed() const
{
  return quantityUsed;
}
