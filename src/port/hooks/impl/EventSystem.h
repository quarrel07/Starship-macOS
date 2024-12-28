#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint16_t EventID;

typedef enum {
    EVENT_TYPE_PRE,
    EVENT_TYPE_POST
} EventType;

typedef enum {
    EVENT_PRIORITY_LOW,
    EVENT_PRIORITY_NORMAL,
    EVENT_PRIORITY_HIGH,
} EventPriority;

typedef struct {
    bool cancelled;
} IEvent;

typedef void (*EventCallback)(IEvent*);

// ID              Type
// 000000000000000 0
#define EVENT_ID(id, type) ((id << 1) | type)

#ifdef __cplusplus
#include <array>
#include <vector>

class EventSystem {
public:
    static EventSystem* Instance;
    size_t RegisterListener(EventID id, EventPriority priority, EventCallback callback);
    void CallEvent(EventID id, IEvent* event);
private:
    std::array<std::vector<EventCallback>, 0xFFFF> mEventListeners;
};
#else
extern size_t EventSystem_RegisterListener(EventID id, EventPriority priority, EventCallback callback);
extern void EventSystem_CallEvent(EventID id, void* event);
#endif