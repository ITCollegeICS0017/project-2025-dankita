#include "managers/ConsumableManager.h"
#include "exceptions/PhotoStudioExceptions.h"

ConsumableManager::ConsumableManager(const IDisplay *disp)
    : display(disp)
{
}

ConsumableManager::~ConsumableManager()
{
  // Clean up consumables (if ownership is  manager)
}

void ConsumableManager::addConsumable(Consumable *consumable)
{
  validateConsumable(consumable);

  consumables.push_back(consumable);
}

void ConsumableManager::recordUsage(const ConsumableUsage &usage)
{
  validateConsumableUsage(usage);

  usageRecords.push_back(usage);

  // Update stock for the consumable
  Consumable *consumable = findConsumableByName(usage.getConsumableName());
  if (consumable)
  {
    int previousStock = consumable->getCurrentStock();

    if (previousStock < usage.getQuantityUsed())
    {
      throw InsufficientStockException(
          "Insufficient stock for " + usage.getConsumableName(),
          "Stock available: " + std::to_string(previousStock) +
              ", requested: " + std::to_string(usage.getQuantityUsed()));
    }

    consumable->updateStock(-usage.getQuantityUsed(), display);

    if (consumable->getCurrentStock() != previousStock - usage.getQuantityUsed())
    {
      throw ValidationException(
          "Stock update failed",
          "Postcondition violation: stock not properly decreased");
    }
  }
}

void ConsumableManager::updateStock(const std::string &consumableName, int quantity)
{
  validateStockUpdate(consumableName, quantity);

  Consumable *consumable = findConsumableByName(consumableName);
  if (consumable)
  {
    int previousStock = consumable->getCurrentStock();
    consumable->updateStock(quantity, display);

    int expectedStock = previousStock + quantity;
    if (consumable->getCurrentStock() != expectedStock)
    {
      throw ValidationException(
          "Stock update failed",
          "Postcondition violation: stock = " + std::to_string(consumable->getCurrentStock()) +
              ", expected = " + std::to_string(expectedStock));
    }
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

void ConsumableManager::validateConsumable(Consumable *consumable) const
{
  if (consumable == nullptr)
  {
    throw InvalidDataException(
        "Invalid consumable data",
        "Precondition violation: consumable == nullptr");
  }

  if (consumable->getName().empty())
  {
    throw InvalidDataException(
        "Consumable name cannot be empty",
        "Precondition violation: consumable name is empty");
  }

  for (const auto &existing : consumables)
  {
    if (existing->getConsumableID() == consumable->getConsumableID())
    {
      throw DuplicateDataException(
          "Consumable ID already exists: " + consumable->getConsumableID(),
          "Duplicate consumable ID in repository");
    }
  }
}

void ConsumableManager::validateConsumableUsage(const ConsumableUsage &usage) const
{
  if (usage.getConsumableName().empty())
  {
    throw InvalidDataException(
        "Consumable name cannot be empty",
        "Precondition violation: consumable name is empty");
  }

  if (usage.getQuantityUsed() <= 0)
  {
    throw InvalidDataException(
        "Usage quantity must be greater than zero",
        "Precondition violation: quantity <= 0");
  }

  Consumable *consumable = const_cast<ConsumableManager *>(this)->findConsumableByName(usage.getConsumableName());
  if (consumable == nullptr)
  {
    throw DataNotFoundException(
        "Consumable not found: " + usage.getConsumableName(),
        "Repository check failed: consumable does not exist");
  }
}

void ConsumableManager::validateStockUpdate(const std::string &consumableName, int quantity) const
{
  if (consumableName.empty())
  {
    throw InvalidDataException(
        "Consumable name cannot be empty",
        "Precondition violation: consumable name is empty");
  }

  Consumable *consumable = const_cast<ConsumableManager *>(this)->findConsumableByName(consumableName);
  if (consumable == nullptr)
  {
    throw DataNotFoundException(
        "Consumable not found: " + consumableName,
        "Repository check failed: consumable does not exist");
  }

  int newStock = consumable->getCurrentStock() + quantity;
  if (newStock < 0)
  {
    throw BusinessRuleException(
        "Cannot reduce stock below zero",
        "Business rule violation: stock cannot be negative (current: " +
            std::to_string(consumable->getCurrentStock()) + ", change: " + std::to_string(quantity) + ")");
  }
}
