//
// Created by anarion on 5/1/20.
//

#ifndef MYMVC_LRUPAYLOADMANAGER_H
#define MYMVC_LRUPAYLOADMANAGER_H

#include "Payload.h"

namespace anarion {
    class LRUPayloadManager {
    protected:
        LinkedList<Payload*> lruList;
        HashMap<Payload*, LinkedList<Payload*>::iterator> payloadMap;
        size_type maxSize;

    public:
        explicit LRUPayloadManager(size_type maxSize) : maxSize(maxSize) {}

        void load(Payload *payload);
    };
}

#endif //MYMVC_LRUPAYLOADMANAGER_H
