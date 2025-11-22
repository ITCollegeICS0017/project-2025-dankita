#include "implementations/ConsoleDisplay.h"
#include <iostream>

void ConsoleDisplay::show(const std::string &message) const
{
  std::cout << message;
}

void ConsoleDisplay::showLine(const std::string &message) const
{
  std::cout << message << std::endl;
}
