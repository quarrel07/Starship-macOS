#pragma once

#include "port/hooks/impl/EventSystem.h"

typedef struct {
    IEvent event;
    int actorId;
} PlayerShotEvent;

#define EVENT_PLAYER_SHOT EVENT_ID(1, EVENT_TYPE_NORMAL)