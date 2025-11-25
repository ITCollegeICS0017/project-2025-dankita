#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>

class Employee
{
protected:
  std::string employeeID;
  std::string name;

public:
  Employee(const std::string &id, const std::string &n);
  virtual ~Employee() = default;

  std::string getName() const;
  std::string getID() const;

  virtual std::string getRole() const = 0;
};

#endif // EMPLOYEE_H
