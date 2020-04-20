//
// Created by anarion on 4/20/20.
//

#include "resource/TextPayload.h"


anarion::TextPayload::TextPayload(anarion::SString text) : text(move(text)) {}

void anarion::TextPayload::send(anarion::SocketChannel &channel) {
    size_type len = text.length();
    channel.in(text.getArr(), len);
}

anarion::size_type anarion::TextPayload::length() {
    return text.length();
}

bool anarion::TextPayload::isLoaded() {
    return true;
}

anarion::SString anarion::TextPayload::defaultType("text/plain"), anarion::TextPayload::defaultEncoding("identity");

const anarion::SString &anarion::TextPayload::getContentType() {
    return defaultType;
}

const anarion::SString &anarion::TextPayload::getContentEncoding() {
    return defaultEncoding;
}

