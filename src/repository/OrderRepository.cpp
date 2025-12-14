#include "repository/OrderRepository.h"
#include "exceptions/PhotoStudioExceptions.h"

OrderRepository::OrderRepository()
    : records(nullptr), count(0), capacity(INITIAL_CAPACITY)
{
  records = new OrderRecord[capacity];
}

OrderRepository::~OrderRepository()
{
  delete[] records;
  records = nullptr;
  count = 0;
  capacity = 0;
}

/**
 * Grow - Implement dynamic growth of the array (Step 4)
 *
 * When the repository becomes full:
 * 1. Double the capacity
 * 2. Allocate a new array
 * 3. Copy elements from old to new
 * 4. Release old memory
 */
void OrderRepository::grow()
{
  int newCapacity = capacity * 2;
  OrderRecord *newRecords = new OrderRecord[newCapacity];

  for (int i = 0; i < count; i++)
  {
    newRecords[i] = records[i];
  }

  delete[] records;

  records = newRecords;
  capacity = newCapacity;
}

void OrderRepository::add(const OrderRecord &record)
{
  if (count >= capacity)
  {
    grow();
  }

  records[count] = record;
  count++;
}

int OrderRepository::getCount() const
{
  return count;
}

OrderRecord &OrderRepository::getAt(int index)
{
  if (index < 0 || index >= count)
  {
    throw DataNotFoundException(
        "Order record not found at index " + std::to_string(index),
        "Index out of bounds: index=" + std::to_string(index) +
            ", count=" + std::to_string(count));
  }
  return records[index];
}

const OrderRecord &OrderRepository::getAt(int index) const
{
  if (index < 0 || index >= count)
  {
    throw DataNotFoundException(
        "Order record not found at index " + std::to_string(index),
        "Index out of bounds: index=" + std::to_string(index) +
            ", count=" + std::to_string(count));
  }
  return records[index];
}

bool OrderRepository::existsById(const std::string &orderID) const
{
  for (int i = 0; i < count; i++)
  {
    if (records[i].orderID == orderID)
    {
      return true;
    }
  }
  return false;
}

int OrderRepository::findIndexById(const std::string &orderID) const
{
  for (int i = 0; i < count; i++)
  {
    if (records[i].orderID == orderID)
    {
      return i;
    }
  }
  return -1;
}

void OrderRepository::updateAt(int index, const OrderRecord &record)
{
  if (index < 0 || index >= count)
  {
    throw DataNotFoundException(
        "Cannot update - order record not found at index " + std::to_string(index),
        "Index out of bounds: index=" + std::to_string(index) +
            ", count=" + std::to_string(count));
  }
  records[index] = record;
}

void OrderRepository::clear()
{
  count = 0;
  // We don't deallocate memory, just reset count
  // This allows reuse without reallocation
}

int OrderRepository::getCapacity() const
{
  return capacity;
}
