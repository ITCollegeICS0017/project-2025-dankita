#ifndef CONSUMABLE_MANAGER_H
#define CONSUMABLE_MANAGER_H

#include <vector>
#include <string>
#include "entities/Consumable.h"
#include "entities/ConsumableUsage.h"
#include "interfaces/IDisplay.h"

class ConsumableManager
{
private:
  std::vector<Consumable *> consumables;
  std::vector<ConsumableUsage> usageRecords;
  const IDisplay *display;

public:
  ConsumableManager(const IDisplay *disp);
  ~ConsumableManager();

  void addConsumable(Consumable *consumable);
  void recordUsage(const ConsumableUsage &usage);
  void updateStock(const std::string &consumableName, int quantity);
  Consumable *findConsumableByName(const std::string &name);

  const std::vector<Consumable *> &getAllConsumables() const;
  const std::vector<ConsumableUsage> &getUsageRecords() const;
};

#endif // CONSUMABLE_MANAGER_H
