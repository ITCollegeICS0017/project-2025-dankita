#ifndef PHOTOSTUDIO_EXCEPTIONS_H
#define PHOTOSTUDIO_EXCEPTIONS_H

#include <exception>
#include <string>

class PhotoStudioException : public std::exception
{
protected:
  std::string message;
  std::string technicalMessage;

public:
  PhotoStudioException(const std::string &msg, const std::string &techMsg = "")
      : message(msg), technicalMessage(techMsg) {}

  const char *what() const noexcept override
  {
    return message.c_str();
  }

  std::string getUserMessage() const { return message; }
  std::string getTechnicalMessage() const { return technicalMessage; }
};

/**
 * InvalidInputException - Thrown for invalid user input at UI layer
 * Examples: empty fields, invalid format
 */
class InvalidInputException : public PhotoStudioException
{
public:
  InvalidInputException(const std::string &msg)
      : PhotoStudioException(msg, "Invalid input provided by user") {}
};

/**
 * InvalidDataException - Thrown for invalid data in Logic layer
 * Examples: negative quantities, invalid IDs, business rule violations
 */
class InvalidDataException : public PhotoStudioException
{
public:
  InvalidDataException(const std::string &msg, const std::string &techMsg = "")
      : PhotoStudioException(msg, techMsg) {}
};

/**
 * ValidationException - Thrown when validation rules fail
 * Examples: precondition violations, postcondition failures
 */
class ValidationException : public PhotoStudioException
{
public:
  ValidationException(const std::string &msg, const std::string &techMsg = "")
      : PhotoStudioException(msg, techMsg) {}
};

/**
 * BusinessRuleException - Thrown when business rules are violated
 * Examples: insufficient stock, invalid order status transition
 */
class BusinessRuleException : public PhotoStudioException
{
public:
  BusinessRuleException(const std::string &msg, const std::string &techMsg = "")
      : PhotoStudioException(msg, techMsg) {}
};

/**
 * DataNotFoundException - Thrown when required data is not found
 * Examples: order not found, consumable not found, client not found
 */
class DataNotFoundException : public PhotoStudioException
{
public:
  DataNotFoundException(const std::string &msg, const std::string &techMsg = "")
      : PhotoStudioException(msg, techMsg) {}
};

/**
 * InsufficientStockException - Thrown when consumable stock is insufficient
 */
class InsufficientStockException : public PhotoStudioException
{
public:
  InsufficientStockException(const std::string &msg, const std::string &techMsg = "")
      : PhotoStudioException(msg, techMsg) {}
};

/**
 * DuplicateDataException - Thrown when attempting to create duplicate records
 * Examples: duplicate order ID, duplicate client ID
 */
class DuplicateDataException : public PhotoStudioException
{
public:
  DuplicateDataException(const std::string &msg, const std::string &techMsg = "")
      : PhotoStudioException(msg, techMsg) {}
};

#endif // PHOTOSTUDIO_EXCEPTIONS_H
