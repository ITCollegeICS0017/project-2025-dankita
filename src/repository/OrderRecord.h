#ifndef ORDER_RECORD_H
#define ORDER_RECORD_H

#include <string>

/**
 * OrderRecord - Entity structure for persistent storage
 *
 * This struct represents one order record as stored in the file.
 * The order of fields matches the file format:
 * orderID|clientID|clientSurname|completionTime|isExpress|status|totalPrice|isPaid
 *
 * File Format (one record per line):
 * O001|C001|Smith|2025-09-01 14:00|0|2|125.00|1
 *
 * Status values: 0=PENDING, 1=IN_PROGRESS, 2=COMPLETED, 3=CANCELLED
 * isExpress/isPaid: 0=false, 1=true
 */
struct OrderRecord
{
  std::string orderID;
  std::string clientID;
  std::string clientSurname;
  std::string completionTime;
  bool isExpress;
  int status; // 0=PENDING, 1=IN_PROGRESS, 2=COMPLETED, 3=CANCELLED
  double totalPrice;
  bool isPaid;

  // Default constructor
  OrderRecord()
      : orderID(""), clientID(""), clientSurname(""), completionTime(""),
        isExpress(false), status(0), totalPrice(0.0), isPaid(false) {}

  // Parameterized constructor
  OrderRecord(const std::string &oID, const std::string &cID,
              const std::string &surname, const std::string &cTime,
              bool express, int stat, double price, bool paid)
      : orderID(oID), clientID(cID), clientSurname(surname),
        completionTime(cTime), isExpress(express), status(stat),
        totalPrice(price), isPaid(paid) {}
};

#endif // ORDER_RECORD_H

