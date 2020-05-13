//
// Created by anarion on 2020/3/31.
//

#include <resource/FilePayload.h>
#include <http/HttpContext.h>
#include "http/HelloApplet.h"

void anarion::HelloApplet::onGet(Request *request, Response *response) {
    Payload *payload = HttpContext::instance().getStaticResources().getPayload(SString("index.html"));
    response->setPayload(payload);
}

