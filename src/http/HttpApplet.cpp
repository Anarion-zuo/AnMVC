//
// Created by anarion on 2020/3/31.
//

#include "http/HttpApplet.h"

anarion::StaticResources anarion::HttpApplet::staticResources(SString("/home/anarion/Documents/CppProjects/MyMVC/static"));

void anarion::HttpApplet::process(Request *request, Response *response) {
    // initialize response
    /*
     * switch Request type
     */
    switch (request->getMethod()) {
        case REQUEST_GET:
            onGet(request, response);
            break;
        case REQUEST_POST:
            onPost(request, response);
            break;
        case REQUEST_PUT:
            onPut(request, response);
            break;
        case REQUEST_DELETE:
            onDelete(request, response);
            break;
        case REQUEST_HEAD:
            onHead(request, response);
            break;
        case REQUEST_CONNECT:
            onConnect(request, response);
            break;
        case REQUEST_OPTIONS:
            onOptions(request, response);
            break;
        case REQUEST_TRACE:
            onTrace(request, response);
            break;
        case REQUEST_PATCH:
            onPatch(request, response);
            break;
        default:
            // TODO unsupported request method
            onUnknown(request, response);
            break;
    }

//    response->send()
}

anarion::TextPayload anarion::HttpApplet::page400Text(SString("Resource not found"));

