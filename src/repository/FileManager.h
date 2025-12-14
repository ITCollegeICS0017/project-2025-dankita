#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "repository/OrderRepository.h"
#include "repository/OrderRecord.h"
#include "interfaces/IDisplay.h"
#include <string>

/**
 * FileManager - Handles file operations for persistent storage
 *
 * This class is responsible for:
 * - Loading data from file at program start (Step 6)
 * - Saving data to file at program end (Step 7)
 *
 * File Format (one record per line, pipe-delimited):
 * orderID|clientID|clientSurname|completionTime|isExpress|status|totalPrice|isPaid
 *
 * Example line:
 * O001|C001|Smith|2025-09-01 14:00|0|2|125.00|1
 */
class FileManager
{
private:
  std::string filePath;
  const IDisplay *display;

  // Helper functions for parsing
  OrderRecord parseLine(const std::string &line) const;
  std::string recordToLine(const OrderRecord &record) const;

public:
  FileManager(const std::string &path, const IDisplay *disp = nullptr);

  /**
   * LoadFromFile - Load data from file at program start (Step 6)
   *
   * - Try to open the file
   * - If successful, read line by line
   * - Convert each line into an OrderRecord
   * - Add to Repository
   * - If line cannot be parsed, skip and continue
   * - If file doesn't exist, start with empty Repository
   */
  bool loadFromFile(OrderRepository &repository);

  /**
   * SaveToFile - Save all data to file at program end (Step 7)
   *
   * - Open file for writing
   * - Write every item in Repository to file
   * - Use consistent file format
   */
  bool saveToFile(const OrderRepository &repository);

  // Getters
  std::string getFilePath() const;
};

#endif // FILE_MANAGER_H

