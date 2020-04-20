//
// Created by anarion on 4/20/20.
//

#include <resource/TextPayload.h>
#include "http/StaticHandler.h"

void anarion::StaticHandler::onGet() {
    SString &dir = request->getDir();
    Payload *payload = staticResources.getPayload(dir);
    if (payload == nullptr) {
        set400Page(404);
        return;
    }
    response->setPayload(payload);
}

anarion::StaticHandler::ItemPool<anarion::StaticHandler> anarion::StaticHandler::pool;

anarion::HttpApplet *anarion::StaticHandler::getInstance() {
    return pool.fetch();
}
