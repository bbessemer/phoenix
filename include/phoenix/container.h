/*
 * Copyright (C) 2017 Brent Bessemer.
 * All rights reserved.
 */

/**
 *
 */

#pragma once

#include <stdlib.h>

namespace px {

template <typename T>
class Container {
protected:
  T *start, *nd, *last;

public:
  typedef T value_type;
  typedef value_type* iterator;
  typedef value_type const* const_iterator;

  Container (size_t n) {
    start = (T *) malloc(n * sizeof(T));
    nd = start + n;
    last = (T *) 0;
  }

  T &operator[] (size_t i) {
    T *ptr = start + i;
    if (ptr < nd) return *ptr;
    else return nd[-1];
  }

  void set (size_t i, value_type &&val) {
    if (i < nd - start)
      start[i] = val;
  }

  template <typename... Args>
  void emplace (size_t i, Args&&... args) {
    if (i < nd - start)
      new (start + i) value_type (args...);
  }

  iterator begin () { return start; }
  iterator end () {
    if (last) return last;
    else return nd;
  }
  const_iterator cbegin () { return (const_iterator) start; }
  const_iterator cend () { return (const_iterator) nd; }

};

template <typename T, size_t N>
class StaticContainer : public Container<T> {
  char space[N * sizeof(T)];

public:
  StaticContainer () {
    start = (T *) space;
    nd = start + N;
    last = (T *) 0;
  }
};

}
