//
// Created by anarion on 2020/3/31.
//

#include <resource/FilePayload.h>
#include <resource/HtmlPayload.h>
#include <resource/CompressedPayload.h>
#include "http/HelloApplet.h"

void anarion::HelloApplet::onGet(Request *request, Response *response) {
    Payload *payload = staticResources.getPayload(SString("index.html"));
    response->setPayload(payload);
}

