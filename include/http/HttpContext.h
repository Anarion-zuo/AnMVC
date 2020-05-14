//
// Created by anarion on 5/13/20.
//

#ifndef MYMVC_HTTPCONTEXT_H
#define MYMVC_HTTPCONTEXT_H

#include <context/Context.h>
#include <listener/TcpPortListener.h>
#include "HttpDispatcher.h"
#include "RequestRouter.h"
#include "ErrorHandler.h"
#include <logger/MvcLogger.h>

namespace anarion {
    class HttpContext : public Context {
        static HttpContext *pInstance;

        TcpPortListener *listener;
        HttpDispatcher *dispatcher;
        StaticResources *staticResources;
        StaticHandler *staticHandler;

        // routing
        RequestRouter *router;
        HashMap<SString, HttpApplet*> routeCacheMap;
        virtual void initRequestRouter() {}

        // error handling
        HashMap<int, ErrorHandler*> errorHandlerMap;
        virtual void initErrorHandler();

        // logger
        MvcLogger logger;

    public:
        explicit HttpContext(in_port_t portNum);

        void preRun() override;

        ~HttpContext() {
            delete listener;
            delete dispatcher;
            delete staticResources;
        }

        void run() override;

        HttpDispatcher &getDispatcher() { return *dispatcher; }
        StaticResources &getStaticResources() { return *staticResources; }
        RequestRouter &getRequestRouter() { return *router; }
        StaticHandler &getStaticHandler() { return *staticHandler; }

        // routing
        HttpApplet *getAppletByRoute(const SString &dir);

        // error handling
        ErrorHandler *getErrorHandlerByCode(int code);

    };
}

#endif //MYMVC_HTTPCONTEXT_H
