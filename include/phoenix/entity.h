/*
 * Copyright (C) 2017 Brent Bessemer.
 * All rights reserved.
 */

/**
 *
 */

#pragma once

namespace px {

class Entity {
  static int last_id;
  int NewId ();

public:
  const int id;
  Entity ();
};

}
