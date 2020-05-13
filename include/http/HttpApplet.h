//
// Created by anarion on 2020/3/31.
//

#ifndef MYMVC_HTTPAPPLET_H
#define MYMVC_HTTPAPPLET_H

#include <concurrent/container/Queue/BlockQueue.hpp>
#include <resource/StaticResources.h>
#include <resource/TextPayload.h>
#include "Request.h"
#include "Response.h"

namespace anarion {
    class HttpApplet {
    protected:

        /*
         * request handling interface
         */
        virtual void onGet(Request *request, Response *response) {}
        virtual void onPost(Request *request, Response *response) {}
        virtual void onDelete(Request *request, Response *response) {}
        virtual void onPut(Request *request, Response *response) {}
        virtual void onHead(Request *request, Response *response) {}
        virtual void onConnect(Request *request, Response *response) {}
        virtual void onOptions(Request *request, Response *response) {}
        virtual void onTrace(Request *request, Response *response) {}
        virtual void onPatch(Request *request, Response *response) {}
        virtual void onUnknown(Request *request, Response *response) {}

        static StaticResources staticResources;

    public:

        HttpApplet() = default;
        virtual ~HttpApplet() = default;

        virtual void process(Request *request, Response *response);
    };
}

#endif //MYMVC_HTTPAPPLET_H
