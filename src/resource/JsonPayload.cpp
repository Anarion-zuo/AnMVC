//
// Created by anarion on 5/1/20.
//

#include "resource/JsonPayload.h"

anarion::SString anarion::JsonPayload::contentType("application/json"), anarion::JsonPayload::contentEncoding("identity");

void anarion::JsonPayload::send(anarion::SocketChannel &channel) {
    channel.in(serialized.getArr(), serialized.length());
}

anarion::size_type anarion::JsonPayload::length() {
    return serialized.length();
}

void anarion::JsonPayload::load() {
    if (jsonObj == nullptr) { return; }
    serialized = jsonObj->toString();
}

void anarion::JsonPayload::unload() {
    serialized.clear();
}

bool anarion::JsonPayload::isLoaded() {
    return serialized.size();
}

const anarion::SString &anarion::JsonPayload::getContentType() {
    return contentType;
}

const anarion::SString &anarion::JsonPayload::getContentEncoding() {
    return contentEncoding;
}
