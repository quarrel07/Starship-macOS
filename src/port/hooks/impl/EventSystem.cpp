#include "EventSystem.h"

EventSystem* EventSystem::Instance = new EventSystem();

size_t EventSystem::RegisterListener(EventID id, EventPriority priority, EventCallback callback) {
    if (std::find(this->mEventListeners[id].begin(), this->mEventListeners[id].end(), callback) != this->mEventListeners[id].end()) {
        throw std::runtime_error("Listener already registered");
    }

    this->mEventListeners[id].push_back(callback);
}

void EventSystem::CallEvent(EventID id, IEvent* event) {
    if (this->mEventListeners[id].empty()) {
        return;
    }

    for (auto& callback : this->mEventListeners[id]) {
        callback(event);
    }
}

extern "C" size_t EventSystem_RegisterListener(EventID id, EventPriority priority, EventCallback callback) {
    return EventSystem::Instance->RegisterListener(id, priority, callback);
}

extern "C" void EventSystem_CallEvent(EventID id, void* event) {
    EventSystem::Instance->CallEvent(id, static_cast<IEvent*>(event));
}