#ifndef CONSUMABLE_H
#define CONSUMABLE_H

#include <string>
#include "interfaces/IDisplay.h"
#include "exceptions/PhotoStudioExceptions.h"

class Consumable
{
private:
  std::string consumableID;
  std::string name;
  int currentStock;
  std::string unitOfMeasure;

public:
  Consumable(const std::string &id, const std::string &n, int stock, const std::string &unit);

  void updateStock(int quantity, const IDisplay *display);

  int getCurrentStock() const;
  std::string getName() const;
  std::string getConsumableID() const;
  std::string getUnitOfMeasure() const;
};

#endif // CONSUMABLE_H
