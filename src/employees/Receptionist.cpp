#include "employees/Receptionist.h"
#include "orders/Order.h"

Receptionist::Receptionist(const std::string &id, const std::string &n)
    : Employee(id, n)
{
}

void Receptionist::createOrder(Client *client, Order *order, const IDisplay *display)
{
  if (display)
  {
    display->showLine("Receptionist " + name + " creating order for client " + client->getSurname());
  }
}

void Receptionist::generateDailyRevenueReport(const IDisplay *display)
{
  if (display)
  {
    display->showLine("Generating daily revenue report...");
  }
}

std::string Receptionist::getRole() const
{
  return "Receptionist";
}
