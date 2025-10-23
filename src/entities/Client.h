#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client
{
private:
  std::string clientID;
  std::string surname;

public:
  Client(const std::string &id, const std::string &sname);

  std::string getSurname() const;
  std::string getID() const;
};

#endif // CLIENT_H
