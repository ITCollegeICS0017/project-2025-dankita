#ifndef CONSUMABLE_USAGE_H
#define CONSUMABLE_USAGE_H

#include <string>
#include "exceptions/PhotoStudioExceptions.h"

class ConsumableUsage
{
private:
  std::string usageID;
  std::string consumableName;
  int quantityUsed;

public:
  ConsumableUsage(const std::string &id, const std::string &name, int qty);

  std::string getUsageID() const;
  std::string getConsumableName() const;
  int getQuantityUsed() const;
};

#endif // CONSUMABLE_USAGE_H
