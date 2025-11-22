#ifndef IDISPLAY_H
#define IDISPLAY_H

#include <string>

class IDisplay
{
public:
  virtual ~IDisplay() = default;
  virtual void show(const std::string &message) const = 0;
  virtual void showLine(const std::string &message) const = 0;
};

#endif // IDISPLAY_H
