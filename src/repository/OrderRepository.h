#ifndef ORDER_REPOSITORY_H
#define ORDER_REPOSITORY_H

#include "repository/OrderRecord.h"
#include "exceptions/PhotoStudioExceptions.h"
#include <string>

/**
 * OrderRepository - Repository with dynamic array
 *
 * This class manages order records in memory using a dynamic array.
 * It contains:
 * - A pointer to the dynamic array
 * - The current number of elements (count)
 * - The current capacity
 *
 * When the array becomes full, capacity is doubled and elements are copied.
 */
class OrderRepository
{
private:
  OrderRecord *records; // Pointer to dynamic array
  int count;            // Current number of elements
  int capacity;         // Current capacity

  static const int INITIAL_CAPACITY = 4;

  // Private helper for array growth
  void grow();

public:
  OrderRepository();
  ~OrderRepository();

  // Disable copy to prevent double-free issues
  OrderRepository(const OrderRepository &) = delete;
  OrderRepository &operator=(const OrderRepository &) = delete;

  void add(const OrderRecord &record);
  int getCount() const;
  OrderRecord &getAt(int index);
  const OrderRecord &getAt(int index) const;

  bool existsById(const std::string &orderID) const;
  int findIndexById(const std::string &orderID) const;
  void updateAt(int index, const OrderRecord &record);
  void clear();

  int getCapacity() const;
};

#endif // ORDER_REPOSITORY_H
