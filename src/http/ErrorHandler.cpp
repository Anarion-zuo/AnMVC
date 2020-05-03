//
// Created by anarion on 5/2/20.
//

#include "http/ErrorHandler.h"

void anarion::ErrorHandler::onAll() {
    response->setPayload(payload);
}

void anarion::ErrorHandler::onGet() {
    onAll();
}

void anarion::ErrorHandler::onPost() {
    onAll();
}

void anarion::ErrorHandler::onDelete() {
    onAll();
}

void anarion::ErrorHandler::onPut() {
    onAll();
}

void anarion::ErrorHandler::onHead() {
    onAll();
}

void anarion::ErrorHandler::onConnect() {
    onAll();
}

void anarion::ErrorHandler::onOptions() {
    onAll();
}

void anarion::ErrorHandler::onTrace() {
    onAll();
}

void anarion::ErrorHandler::onPatch() {
    onAll();
}

void anarion::ErrorHandler::onUnknown() {
    onAll();
}

anarion::HashMap<int, anarion::ErrorHandler*> anarion::ErrorHandler::code2handler;
anarion::Mutex anarion::ErrorHandler::mapLock;

anarion::ErrorHandler *anarion::ErrorHandler::getHandlerByStatusCode(int code) {
    mapLock.lock();
    if (code2handler.empty()) {
        initCode2HandlerMap();
    }
    auto it = code2handler.find(code);
    ErrorHandler *ret = nullptr;
    if (it != code2handler.end_iterator()) {
        ret = it->get_val();
    }
    mapLock.unlock();
    return ret;
}

void anarion::ErrorHandler::initCode2HandlerMap() {
    auto &map = code2handler;

    map.put(503, new ErrorHandler(503, new TextPayload(SString("503 Service Unavailable"))));
    map.put(400, new ErrorHandler(400, new TextPayload(SString("400 Bad Request"))));
    map.put(404, new ErrorHandler(404, new TextPayload(SString("404 Resource not found"))));
}

anarion::HttpApplet *anarion::ErrorHandler::getInstance() {
    return nullptr;
}

void anarion::ErrorHandler::release() {

}

void anarion::ErrorHandler::process() {
    processLock.lock();
    HttpApplet::process();
    processLock.unlock();
}
