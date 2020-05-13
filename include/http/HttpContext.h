//
// Created by anarion on 5/13/20.
//

#ifndef MYMVC_HTTPCONTEXT_H
#define MYMVC_HTTPCONTEXT_H

#include <context/Context.h>
#include <listener/TcpPortListener.h>
#include "HttpDispatcher.h"
#include "RequestRouter.h"

namespace anarion {
    class HttpContext : public Context {
        static HttpContext *pInstance;

        TcpPortListener *listener;
        HttpDispatcher *dispatcher;
        StaticResources *staticResources;
        RequestRouter *router;

        virtual void initMappings() {}

    public:

        static HttpContext &instance() { return *pInstance; }

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
    };
}

#endif //MYMVC_HTTPCONTEXT_H
