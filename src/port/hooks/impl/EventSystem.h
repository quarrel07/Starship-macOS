#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint16_t EventID;
typedef uint32_t ListenerID;

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

typedef struct {
    EventPriority priority;
    EventCallback function;
} EventListener;

// ID              Type
// 000000000000000 0
#define EVENT_ID(id, type) ((id << 1) | type)

#ifdef __cplusplus
#include <array>
#include <vector>

class EventSystem {
public:
    static EventSystem* Instance;
    ListenerID RegisterListener(EventID id, EventCallback callback, EventPriority priority = EVENT_PRIORITY_NORMAL);
    void UnregisterListener(EventID ev, ListenerID id);
    void CallEvent(EventID id, IEvent* event);
private:
    std::array<std::vector<EventListener>, 0xFFFF> mEventListeners;
};
#else
extern ListenerID EventSystem_RegisterListener(EventID id, EventCallback callback, EventPriority priority);
extern void EventSystem_UnregisterListener(EventID ev, ListenerID id);
extern void EventSystem_CallEvent(EventID id, void* event);
#endif