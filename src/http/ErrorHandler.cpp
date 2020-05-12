//
// Created by anarion on 5/2/20.
//

#include "http/ErrorHandler.h"

void anarion::ErrorHandler::onAll(Request *request, Response *response) {
    response->setPayload(payload);
}

void anarion::ErrorHandler::onGet(Request *request, Response *response) {
    onAll(request, response);
}

void anarion::ErrorHandler::onPost(Request *request, Response *response) {
    onAll(request, response);
}

void anarion::ErrorHandler::onDelete(Request *request, Response *response) {
    onAll(request, response);
}

void anarion::ErrorHandler::onPut(Request *request, Response *response) {
    onAll(request, response);
}

void anarion::ErrorHandler::onHead(Request *request, Response *response) {
    onAll(request, response);
}

void anarion::ErrorHandler::onConnect(Request *request, Response *response) {
    onAll(request, response);
}

void anarion::ErrorHandler::onOptions(Request *request, Response *response) {
    onAll(request, response);
}

void anarion::ErrorHandler::onTrace(Request *request, Response *response) {
    onAll(request, response);
}

void anarion::ErrorHandler::onPatch(Request *request, Response *response) {
    onAll(request, response);
}

void anarion::ErrorHandler::onUnknown(Request *request, Response *response) {
    onAll(request, response);
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

void anarion::ErrorHandler::process(Request *request, Response *response) {
    processLock.lock();
    HttpApplet::process(request, response);
    processLock.unlock();
}
