//
// Created by anarion on 5/13/20.
//

#include <http/HelloApplet.h>
#include "http/HttpContext.h"
#include <controller/Controller.h>

anarion::HttpContext *anarion::HttpContext::pInstance = nullptr;

void anarion::HttpContext::run() {
    listener->launch();
    while (true) {
        dispatcher->process();
    }
}

anarion::HttpContext::HttpContext(in_port_t portNum) :
    listener(new TcpPortListener(portNum)),
    dispatcher(new HttpDispatcher(this, listener)),
    staticResources(new StaticResources(SString("/home/anarion/Documents/CppProjects/MyMVC/static"))),
    staticHandler(new StaticHandler(this)),
    router(new RequestRouter(this)),
    logger(SString("/home/anarion/Documents/CppProjects/MyMVC/web/mvc-log.txt"))
{
    // update instance
    delete pInstance;
    pInstance = this;
}

void anarion::HttpContext::preRun() {
    // init mappings
    dispatcher->addMapping(SString("/"), new HelloApplet(this));
    initRequestRouter();
    initErrorHandler();
}

anarion::HttpApplet *anarion::HttpContext::getAppletByRoute(const anarion::SString &dir) {
    auto cacheIt = routeCacheMap.find(dir);
    if (cacheIt != routeCacheMap.end_iterator()) {
        return cacheIt->get_val();
    }
    HttpApplet *applet = router->query(dir);
    if (applet == nullptr) {
        applet = staticHandler;
    }
    routeCacheMap.put(dir, applet);
    return applet;
}

void anarion::HttpContext::initErrorHandler() {
    errorHandlerMap.put(503, new ErrorHandler(this, 503, new TextPayload(SString("503 Service Unavailable"))));
    errorHandlerMap.put(400, new ErrorHandler(this, 400, new TextPayload(SString("400 Bad Request"))));
    errorHandlerMap.put(404, new ErrorHandler(this, 404, new TextPayload(SString("404 Resource not found"))));
}

anarion::ErrorHandler *anarion::HttpContext::getErrorHandlerByCode(int code) {
    auto it = errorHandlerMap.find(code);
    ErrorHandler *ret = nullptr;
    if (it != errorHandlerMap.end_iterator()) {
        ret = it->get_val();
    }
    return ret;
}
