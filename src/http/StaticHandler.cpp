//
// Created by anarion on 4/20/20.
//

#include <resource/TextPayload.h>
#include "http/StaticHandler.h"

void anarion::StaticHandler::onGet() {
    SString &dir = request->getDir();
    Payload *payload = staticResources.getPayload(dir);
    if (payload == nullptr) {
        throw StaticResourceNotFound();
    }
    response->setPayload(payload);
}

anarion::StaticHandler::ItemPool<anarion::StaticHandler> anarion::StaticHandler::pool;

anarion::HttpApplet *anarion::StaticHandler::getInstance() {
    return pool.fetch();
}

void anarion::StaticHandler::release() {
    pool.returnItem(this);
}

const char *anarion::StaticResourceNotFound::what() const noexcept {
    return "Requested resource not found by server.";
}
