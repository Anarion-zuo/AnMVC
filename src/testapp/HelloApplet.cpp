//
// Created by anarion on 2020/3/31.
//

#include <resource/FilePayload.h>
#include <resource/HtmlPayload.h>
#include <resource/CompressedPayload.h>
#include "http/HelloApplet.h"


anarion::HelloApplet::ItemPool<anarion::HelloApplet> anarion::HelloApplet::pool;

anarion::HttpApplet *anarion::HelloApplet::getInstance() {
    return pool.fetch();
}

void anarion::HelloApplet::onGet() {
    Payload *payload = staticResources.getPayload(SString("hello.html"));
    response->setPayload(payload);
}

void anarion::HelloApplet::release() {
    request = nullptr;
    response = nullptr;
    pool.returnItem(this);
}
