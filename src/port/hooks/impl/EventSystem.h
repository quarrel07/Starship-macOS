#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint16_t EventID;
typedef uint16_t NamespaceID;
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

// Namespace        ID              Type
// 00000000XXXXXXXX 000000000000000 0
#define EVENT_ID(namespace_, id_, type_) ((((uint32_t)(namespace_) & 0xFFFF) << 16) | (((uint32_t)(id_) & 0x7FFF) << 1) | ((uint32_t)(type_) & 0x1))

#define INTERNAL_EVENT_ID(id, type) EVENT_ID(0, id, type)
#define DEFINE_EVENT(id, eventName, type, ...) \
    typedef struct { \
        IEvent event; \
        __VA_ARGS__ \
    } eventName; \
    \
    static uint32_t eventName##_ID = INTERNAL_EVENT_ID(id, type);

#define CALL_EVENT(eventType, ...) \
    eventType eventType##_ = { {false}, __VA_ARGS__ }; \
    EventSystem_CallEvent(eventType##_ID, &eventType##_);

#define CALL_CANCELLABLE_EVENT(eventType, ...) \
    eventType eventType##_ = { {false}, __VA_ARGS__ }; \
    EventSystem_CallEvent(eventType##_ID, &eventType##_); \
    if (!eventType##_.event.cancelled)

#ifdef __cplusplus
#include <array>
#include <vector>
#include <unordered_map>

class EventSystem {
public:
    static EventSystem* Instance;
    ListenerID RegisterListener(EventID id, EventCallback callback, EventPriority priority = EVENT_PRIORITY_NORMAL);
    void UnregisterListener(EventID ev, ListenerID id);
    void CallEvent(EventID id, IEvent* event);
private:
    std::unordered_map<NamespaceID, std::unordered_map<EventID, std::vector<EventListener>>> mEventListeners;
};
#else
extern ListenerID EventSystem_RegisterListener(EventID id, EventCallback callback, EventPriority priority);
extern void EventSystem_UnregisterListener(EventID ev, ListenerID id);
extern void EventSystem_CallEvent(EventID id, void* event);
#endif