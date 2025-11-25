#ifndef SERVICE_H
#define SERVICE_H

#include <string>
#include "types/Types.h"

class Service
{
private:
  std::string serviceID;
  std::string name;
  double basePrice;
  ServiceType type;

public:
  Service(const std::string &id, const std::string &n, double price, ServiceType t);

  double getBasePrice() const;
  std::string getName() const;
  std::string getServiceID() const;
  ServiceType getType() const;
};

#endif // SERVICE_H
