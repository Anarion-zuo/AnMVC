//
// Created by anarion on 5/2/20.
//

#ifndef MYMVC_ERRORHANDLER_H
#define MYMVC_ERRORHANDLER_H

#include "HttpApplet.h"

namespace anarion {
    class ErrorHandler : public HttpApplet {
    protected:
        int code;

        Payload *payload;

        Mutex processLock;

        void onAll(Request *request, Response *response);

        void onGet(Request *request, Response *response) override;
        void onPost(Request *request, Response *response) override;
        void onDelete(Request *request, Response *response) override;
        void onPut(Request *request, Response *response) override;
        void onHead(Request *request, Response *response) override;
        void onConnect(Request *request, Response *response) override;
        void onOptions(Request *request, Response *response) override;
        void onTrace(Request *request, Response *response) override;
        void onPatch(Request *request, Response *response) override;
        void onUnknown(Request *request, Response *response) override;

    public:
        ErrorHandler(HttpContext *context, int code, Payload *payload) : HttpApplet(context), code(code), payload(payload) {}

        void process(Request *request, Response *response) override;

    };
}

#endif //MYMVC_ERRORHANDLER_H
