//
// Created by anarion on 5/13/20.
//

#include <http/HelloApplet.h>
#include "http/HttpContext.h"

anarion::HttpContext *anarion::HttpContext::pInstance = nullptr;

void anarion::HttpContext::run() {
    listener->launch();
    while (true) {
        dispatcher->process();
    }
}

anarion::HttpContext::HttpContext(in_port_t portNum) :
    listener(new TcpPortListener(portNum)),
    dispatcher(new HttpDispatcher(listener)),
    staticResources(new StaticResources(SString("/home/anarion/Documents/CppProjects/MyMVC/static"))),
    router(new RequestRouter)
{
    // update instance
    delete pInstance;
    pInstance = this;
}

void anarion::HttpContext::preRun() {
    // init mappings
    dispatcher->addMapping(SString("/"), new HelloApplet);
    initMappings();
}
