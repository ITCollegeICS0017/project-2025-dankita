#include "orders/ExpressOrder.h"

ExpressOrder::ExpressOrder(const std::string &id, const std::string &cTime,
                           Client *c, const Config *cfg)
    : Order(id, cTime, c), config(cfg)
{
}

double ExpressOrder::calculatePrice()
{
  double basePrice = Order::calculatePrice();
  double surchargeRate = config ? config->getExpressSurchargeRate() : 0.25;
  double expressPrice = basePrice * (1.0 + surchargeRate);

  return expressPrice;
}
