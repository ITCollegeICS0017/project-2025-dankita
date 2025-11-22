#include "entities/Report.h"

Report::Report(const std::string &id, ReportType type, const std::string &cont)
    : reportID(id), reportType(type), content(cont)
{
}

std::string Report::getContent() const
{
  return content;
}

ReportType Report::getReportType() const
{
  return reportType;
}

std::string Report::getReportID() const
{
  return reportID;
}
