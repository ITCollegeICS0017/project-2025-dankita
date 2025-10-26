#include "entities/Service.h"

Service::Service(const std::string &id, const std::string &n, double price, ServiceType t)
    : serviceID(id), name(n), basePrice(price), type(t)
{
}

double Service::getBasePrice() const
{
  return basePrice;
}

std::string Service::getName() const
{
  return name;
}

std::string Service::getServiceID() const
{
  return serviceID;
}

ServiceType Service::getType() const
{
  return type;
}
