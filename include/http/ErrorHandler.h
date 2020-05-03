//
// Created by anarion on 5/2/20.
//

#ifndef MYMVC_ERRORHANDLER_H
#define MYMVC_ERRORHANDLER_H

#include "HttpApplet.h"

namespace anarion {
    class ErrorHandler : virtual public HttpApplet {
    protected:
        int code;
        ErrorHandler(int code, Payload *payload) : code(code), payload(payload) {}

        Payload *payload;

        Mutex processLock;

        void onAll();

        void onGet() override;
        void onPost() override;
        void onDelete() override;
        void onPut() override;
        void onHead() override;
        void onConnect() override;
        void onOptions() override;
        void onTrace() override;
        void onPatch() override;
        void onUnknown() override;

        static HashMap<int, ErrorHandler*> code2handler;
        static void initCode2HandlerMap();
        static Mutex mapLock;
    public:
        static ErrorHandler *getHandlerByStatusCode(int code);

        void release() override;

        void process() override;

        HttpApplet *getInstance() override;
    };
}

#endif //MYMVC_ERRORHANDLER_H
