#ifndef EXPRESS_ORDER_H
#define EXPRESS_ORDER_H

#include "orders/Order.h"
#include "config/Config.h"

class ExpressOrder : public Order
{
private:
  const Config *config;

public:
  ExpressOrder(const std::string &id, const std::string &cTime,
               Client *c, const Config *cfg);

  // Override for polymorphic behavior
  double calculatePrice() override;
};

#endif // EXPRESS_ORDER_H
