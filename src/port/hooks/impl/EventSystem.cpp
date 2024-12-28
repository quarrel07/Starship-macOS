#include "EventSystem.h"
#include <stdexcept>
#include <algorithm>

EventSystem* EventSystem::Instance = new EventSystem();

ListenerID EventSystem::RegisterListener(EventID id, EventCallback callback, EventPriority priority) {
    auto& listeners = this->mEventListeners[(uint8_t)((id >> 16) & 0xFF)][(uint16_t)(id & 0xFFFF)];

    if(std::find_if(listeners.begin(), listeners.end(), [callback](EventListener listener) {
        return listener.function == callback;
    }) != listeners.end()) {
        throw std::runtime_error("Listener already registered");
    }

    listeners.push_back({ priority, callback });

    // Sort by priority
    std::sort(listeners.begin(), listeners.end(), [](EventListener a, EventListener b) {
        return a.priority < b.priority;
    });

    return listeners.size() - 1;
}

void EventSystem::UnregisterListener(EventID id, ListenerID listenerId) {
    auto& listeners = this->mEventListeners[(uint8_t)((id >> 16) & 0xFF)][(uint16_t)(id & 0xFFFF)];

    listeners.erase(listeners.begin() + listenerId);
}

void EventSystem::CallEvent(EventID id, IEvent* event) {
    auto& listeners = this->mEventListeners[(uint8_t)((id >> 16) & 0xFF)][(uint16_t)(id & 0xFFFF)];

    if (listeners.empty()) {
        return;
    }

    for (auto& listener : listeners) {
        listener.function(event);
    }
}

extern "C" size_t EventSystem_RegisterListener(EventID id, EventCallback callback, EventPriority priority) {
    return EventSystem::Instance->RegisterListener(id, callback, priority);
}

extern "C" void EventSystem_UnregisterListener(EventID ev, size_t id) {
    EventSystem::Instance->UnregisterListener(ev, id);
}

extern "C" void EventSystem_CallEvent(EventID id, void* event) {
    EventSystem::Instance->CallEvent(id, static_cast<IEvent*>(event));
}