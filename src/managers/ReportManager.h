#ifndef REPORT_MANAGER_H
#define REPORT_MANAGER_H

#include <vector>
#include <string>
#include "entities/Report.h"
#include "interfaces/IDisplay.h"

class OrderManager;
class ConsumableManager;

class ReportManager
{
private:
  std::vector<Report *> reports;
  const IDisplay *display;

public:
  ReportManager(const IDisplay *disp);
  ~ReportManager();

  void generateDailyRevenueReport(const OrderManager *orderManager);
  void generateConsumablesUsageReport(const ConsumableManager *consumableManager);

  const std::vector<Report *> &getAllReports() const;
};

#endif // REPORT_MANAGER_H
