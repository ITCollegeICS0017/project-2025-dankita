#include "managers/ConsumableManager.h"

ConsumableManager::ConsumableManager(const IDisplay *disp)
    : display(disp)
{
}

ConsumableManager::~ConsumableManager()
{
  // Clean up consumables (if ownership is with the manager)
}

void ConsumableManager::addConsumable(Consumable *consumable)
{
  consumables.push_back(consumable);
}

void ConsumableManager::recordUsage(const ConsumableUsage &usage)
{
  usageRecords.push_back(usage);

  // Update stock for the consumable
  Consumable *consumable = findConsumableByName(usage.getConsumableName());
  if (consumable)
  {
    consumable->updateStock(-usage.getQuantityUsed(), display);
  }
}

void ConsumableManager::updateStock(const std::string &consumableName, int quantity)
{
  Consumable *consumable = findConsumableByName(consumableName);
  if (consumable)
  {
    consumable->updateStock(quantity, display);
  }
}

Consumable *ConsumableManager::findConsumableByName(const std::string &name)
{
  for (auto *consumable : consumables)
  {
    if (consumable->getName() == name)
    {
      return consumable;
    }
  }
  return nullptr;
}

const std::vector<Consumable *> &ConsumableManager::getAllConsumables() const
{
  return consumables;
}

const std::vector<ConsumableUsage> &ConsumableManager::getUsageRecords() const
{
  return usageRecords;
}
