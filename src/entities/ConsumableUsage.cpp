#include "entities/ConsumableUsage.h"

ConsumableUsage::ConsumableUsage(const std::string &id, const std::string &name, int qty)
    : usageID(id), consumableName(name), quantityUsed(qty)
{
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
