#pragma once

#include "port/hooks/impl/EventSystem.h"

#define DisplayPreUpdateEvent  INTERNAL_EVENT_ID(1, EVENT_TYPE_PRE)
#define DisplayPostUpdateEvent INTERNAL_EVENT_ID(1, EVENT_TYPE_POST)

#define GamePreUpdateEvent  INTERNAL_EVENT_ID(2, EVENT_TYPE_PRE)
#define GamePostUpdateEvent INTERNAL_EVENT_ID(2, EVENT_TYPE_POST)