#ifndef REPORT_H
#define REPORT_H

#include <string>
#include "types/Types.h"

class Report
{
private:
  std::string reportID;
  ReportType reportType;
  std::string content;

public:
  Report(const std::string &id, ReportType type, const std::string &cont);

  std::string getContent() const;
  ReportType getReportType() const;
  std::string getReportID() const;
};

#endif // REPORT_H
