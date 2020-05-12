//
// Created by anarion on 4/20/20.
//

#include <resource/TextPayload.h>
#include "http/StaticHandler.h"

void anarion::StaticHandler::onGet(Request *request, Response *response) {
    SString &dir = request->getDir();
    Payload *payload = staticResources.getPayload(dir);
    if (payload == nullptr) {
        throw StaticResourceNotFound();
    }
    response->setPayload(payload);
}


const char *anarion::StaticResourceNotFound::what() const noexcept {
    return "Requested resource not found by server.";
}
