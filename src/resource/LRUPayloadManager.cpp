//
// Created by anarion on 5/1/20.
//

#include "resource/LRUPayloadManager.h"

void anarion::LRUPayloadManager::load(anarion::Payload *payload) {
    if (maxSize == 0) { return; }

    auto setIt = payloadMap.find(payload);

    if (setIt == payloadMap.end_iterator()) {
        // payload unloaded in manager
        if (lruList.size() == maxSize) {
            // exceeds maximum cache size
            // evicts the last entry
            auto lastIt = lruList.end_iterator() - 1;
            (**lastIt).unload();
            payloadMap.remove(*lastIt);
            payloadMap.put(payload, lastIt);
            *lastIt = payload;
        } else {
            // maximum cache size not exceeded
            auto listIt = lruList.push_front(payload);
            payloadMap.put(payload, listIt);
        }
        payload->load();
    } else {
        // payload loaded in manager
        // move the payload to the front of the list
        LinkedList<Payload*>::iterator listIt = setIt->get_val();
        lruList.move_to_front(listIt);
    }
}
