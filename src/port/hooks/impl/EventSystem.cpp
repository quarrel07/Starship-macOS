#include "EventSystem.h"
#include <stdexcept>

EventSystem* EventSystem::Instance = new EventSystem();

ListenerID EventSystem::RegisterListener(EventID id, EventCallback callback, EventPriority priority) {
    if(std::find_if(this->mEventListeners[id].begin(), this->mEventListeners[id].end(), [callback](EventListener listener) {
        return listener.function == callback;
    }) != this->mEventListeners[id].end()) {
        throw std::runtime_error("Listener already registered");
    }

    this->mEventListeners[id].push_back({ priority, callback });

    // Sort by priority
    std::sort(this->mEventListeners[id].begin(), this->mEventListeners[id].end(), [](EventListener a, EventListener b) {
        return a.priority < b.priority;
    });

    return this->mEventListeners[id].size() - 1;
}

void EventSystem::UnregisterListener(EventID ev, ListenerID id) {
    this->mEventListeners[ev].erase(this->mEventListeners[ev].begin() + id);
}

void EventSystem::CallEvent(EventID id, IEvent* event) {
    if (this->mEventListeners[id].empty()) {
        return;
    }

    for (auto& listener : this->mEventListeners[id]) {
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