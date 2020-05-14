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

void anarion::ErrorHandler::process(Request *request, Response *response) {
    processLock.lock();
    HttpApplet::process(request, response);
    processLock.unlock();
}
