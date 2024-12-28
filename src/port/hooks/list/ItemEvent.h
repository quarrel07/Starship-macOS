#pragma once

#include "gfx.h"
#include "sf64object.h"
#include "port/hooks/impl/EventSystem.h"

DEFINE_EVENT(3, ItemDropEvent, EVENT_TYPE_PRE, 
    Item* item;
);