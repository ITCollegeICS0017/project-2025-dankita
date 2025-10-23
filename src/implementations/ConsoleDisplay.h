#ifndef CONSOLE_DISPLAY_H
#define CONSOLE_DISPLAY_H

#include "interfaces/IDisplay.h"

class ConsoleDisplay : public IDisplay
{
public:
  ConsoleDisplay() = default;
  ~ConsoleDisplay() override = default;

  void show(const std::string &message) const override;
  void showLine(const std::string &message) const override;
};

#endif // CONSOLE_DISPLAY_H
