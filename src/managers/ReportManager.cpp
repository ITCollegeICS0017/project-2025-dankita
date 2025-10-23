#include "managers/ReportManager.h"
#include "managers/OrderManager.h"
#include "managers/ConsumableManager.h"

ReportManager::ReportManager(const IDisplay *disp)
    : display(disp)
{
}

ReportManager::~ReportManager()
{
  // Clean up reports
  for (auto *report : reports)
  {
    delete report;
  }
}

void ReportManager::generateDailyRevenueReport(const OrderManager *orderManager)
{
  double totalRevenue = orderManager->calculateTotalRevenue();
  std::string content = "=== Daily Revenue Report ===\n";
  content += "Total Revenue: $" + std::to_string(totalRevenue) + "\n";
  content += "Total Orders: " + std::to_string(orderManager->getAllOrders().size()) + "\n";

  std::string reportID = "REP_REV_001";
  Report *report = new Report(reportID, ReportType::DAILY_REVENUE, content);
  reports.push_back(report);

  if (display)
  {
    display->showLine("=== Daily Revenue Report ===");
    display->showLine("Total Revenue: $" + std::to_string(totalRevenue));
    display->showLine("Total Orders: " + std::to_string(orderManager->getAllOrders().size()));
  }
}

void ReportManager::generateConsumablesUsageReport(const ConsumableManager *consumableManager)
{
  std::string content = "=== Consumables Usage Report ===\n";

  const auto &usageRecords = consumableManager->getUsageRecords();
  for (const auto &usage : usageRecords)
  {
    content += usage.getConsumableName() + ": " + std::to_string(usage.getQuantityUsed()) + " units\n";
  }

  std::string reportID = "REP_CON_001";
  Report *report = new Report(reportID, ReportType::CONSUMABLES_USAGE, content);
  reports.push_back(report);

  if (display)
  {
    display->showLine("=== Consumables Usage Report ===");
    for (const auto &usage : usageRecords)
    {
      display->showLine(usage.getConsumableName() + ": " + std::to_string(usage.getQuantityUsed()) + " units");
    }
  }
}

const std::vector<Report *> &ReportManager::getAllReports() const
{
  return reports;
}
