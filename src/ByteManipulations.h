#ifndef BYTE_MANIPULATIONS_H_INCLUDED
#define BYTE_MANIPULATIONS_H_INCLUDED

#include <algorithm>
#include <stdint.h>

#include <iostream>
#include <iomanip>

template <typename T>
const uint8_t* ParseValue(const uint8_t* start, T& data, size_t size) {
  const uint8_t* end = start + size;
  std::copy(start, end, reinterpret_cast<uint8_t*>(&data));
  return end;
}


#endif //BYTE_MANIPULATIONS_H_INCLUDED
