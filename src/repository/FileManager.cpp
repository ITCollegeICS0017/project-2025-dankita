#include "repository/FileManager.h"
#include <fstream>
#include <sstream>
#include <iomanip>

FileManager::FileManager(const std::string &path, const IDisplay *disp)
    : filePath(path), display(disp)
{
}

OrderRecord FileManager::parseLine(const std::string &line) const
{
  OrderRecord record;
  std::istringstream stream(line);
  std::string token;
  int fieldIndex = 0;

  while (std::getline(stream, token, '|'))
  {
    switch (fieldIndex)
    {
    case 0:
      record.orderID = token;
      break;
    case 1:
      record.clientID = token;
      break;
    case 2:
      record.clientSurname = token;
      break;
    case 3:
      record.completionTime = token;
      break;
    case 4:
      record.isExpress = (token == "1");
      break;
    case 5:
      record.status = std::stoi(token);
      break;
    case 6:
      record.totalPrice = std::stod(token);
      break;
    case 7:
      record.isPaid = (token == "1");
      break;
    }
    fieldIndex++;
  }

  return record;
}

std::string FileManager::recordToLine(const OrderRecord &record) const
{
  std::ostringstream stream;
  stream << record.orderID << "|"
         << record.clientID << "|"
         << record.clientSurname << "|"
         << record.completionTime << "|"
         << (record.isExpress ? "1" : "0") << "|"
         << record.status << "|"
         << std::fixed << std::setprecision(2) << record.totalPrice << "|"
         << (record.isPaid ? "1" : "0");
  return stream.str();
}

bool FileManager::loadFromFile(OrderRepository &repository)
{
  std::ifstream file(filePath);

  if (!file.is_open())
  {
    if (display)
    {
      display->showLine("Data file not found. Starting with empty database.");
    }
    return false;
  }

  std::string line;
  int loadedCount = 0;
  int skippedCount = 0;

  while (std::getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }

    try
    {
      OrderRecord record = parseLine(line);

      if (record.orderID.empty())
      {
        skippedCount++;
        continue;
      }

      repository.add(record);
      loadedCount++;
    }
    catch (const std::exception &e)
    {
      skippedCount++;
      if (display)
      {
        display->showLine("Warning: Skipped invalid line in data file.");
      }
    }
  }

  file.close();

  if (display)
  {
    display->showLine("Loaded " + std::to_string(loadedCount) + " order(s) from file.");
    if (skippedCount > 0)
    {
      display->showLine("Skipped " + std::to_string(skippedCount) + " invalid line(s).");
    }
  }

  return true;
}

bool FileManager::saveToFile(const OrderRepository &repository)
{
  std::ofstream file(filePath);

  if (!file.is_open())
  {
    if (display)
    {
      display->showLine("Error: Could not open file for writing: " + filePath);
    }
    return false;
  }

  int savedCount = 0;

  for (int i = 0; i < repository.getCount(); i++)
  {
    const OrderRecord &record = repository.getAt(i);
    std::string line = recordToLine(record);
    file << line << "\n";
    savedCount++;
  }

  file.close();

  if (display)
  {
    display->showLine("Saved " + std::to_string(savedCount) + " order(s) to file.");
  }

  return true;
}


std::string FileManager::getFilePath() const
{
  return filePath;
}

