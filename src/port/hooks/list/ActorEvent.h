#pragma once

#include "gfx.h"
#include "sf64object.h"
#include "port/hooks/impl/EventSystem.h"

typedef enum {
    OBJECT_TYPE_ACTOR,
    OBJECT_TYPE_BOSS,
    OBJECT_TYPE_SCENERY,
    OBJECT_TYPE_SPRITE,
    OBJECT_TYPE_ITEM,
    OBJECT_TYPE_EFFECT,
} ObjectEventType;

DEFINE_EVENT(ObjectInitEvent,
    ObjectEventType type;
    void* object;
);

DEFINE_EVENT(ObjectUpdateEvent,
    ObjectEventType type;
    void* object;
);

DEFINE_EVENT(ObjectDrawEvent,
    ObjectEventType type;
    void* object;
);

DEFINE_EVENT(ObjectDestroyEvent,
    ObjectEventType type;
    void* object;
);