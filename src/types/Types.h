#ifndef TYPES_H
#define TYPES_H

#include <string>

enum class OrderStatus
{
  PENDING,
  IN_PROGRESS,
  COMPLETED,
  CANCELLED
};

enum class ReportType
{
  DAILY_REVENUE,
  CONSUMABLES_USAGE
};

enum class ServiceType
{
  PHOTO_PRINTING,
  FILM_DEVELOPING
};

#endif // TYPES_H
