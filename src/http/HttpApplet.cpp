//
// Created by anarion on 2020/3/31.
//

#include "http/HttpApplet.h"

anarion::StaticResources anarion::HttpApplet::staticResources(SString("/home/anarion/Documents/CppProjects/MyMVC/static"));

void anarion::HttpApplet::process() {
    // initialize response
    response = new Response();  // default status 200
    /*
     * switch Request type
     */
    switch (request->getMethod()) {
        case REQUEST_GET:
            onGet();
            break;
        case REQUEST_POST:
            onPost();
            break;
        case REQUEST_PUT:
            onPut();
            break;
        case REQUEST_DELETE:
            onDelete();
            break;
        case REQUEST_HEAD:
            onHead();
            break;
        case REQUEST_CONNECT:
            onConnect();
            break;
        case REQUEST_OPTIONS:
            onOptions();
            break;
        case REQUEST_TRACE:
            onTrace();
            break;
        case REQUEST_PATCH:
            onPatch();
            break;
        default:
            // TODO unsupported request method
            break;
    }

//    response->send()
}

anarion::TextPayload anarion::HttpApplet::page400Text(SString("Resource not found"));

void anarion::HttpApplet::set400Page(int status) {
    response->setStatus(status);
//    staticResources.getPayload(SString("40x.html"));
    response->setPayload(&page400Text);
}
