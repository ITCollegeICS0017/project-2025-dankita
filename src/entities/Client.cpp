#include "entities/Client.h"

Client::Client(const std::string &id, const std::string &sname)
    : clientID(id), surname(sname)
{
}

std::string Client::getSurname() const
{
  return surname;
}

std::string Client::getID() const
{
  return clientID;
}
