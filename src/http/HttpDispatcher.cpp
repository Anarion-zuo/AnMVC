//
// Created by anarion on 2020/3/31.
//

#include <http/HelloApplet.h>
#include <http/ErrorHandler.h>
#include "http/HttpDispatcher.h"

using namespace anarion;


void anarion::HttpDispatcher::process() {
    // obtain channel
    TcpSocketChannel *tcpChannel = listener->pollQueue();
    /*
     * The parser operates directly on the connection
     * To avoid unnecessary copying operations for huge payloads
     */
    // check if connection close
    if (tcpChannel->checkClose()) {
        returnHttpConnection(tcpChannel, nullptr);
        // no cleanup required
        return;
    }
    Request *request = nullptr;
    bool isBadRequest = false;
    try {
        request = Request::parse(*tcpChannel);
    } catch (HttpRequestTextSyntaxError error) {
        isBadRequest = true;
    }

    // map to applet
    SString &requestDir = request->getDir();
    HttpApplet *applet;
    if (isBadRequest) {
        applet = ErrorHandler::getHandlerByStatusCode(400);
    } else {
        HttpApplet *mappedApplet = getMappedApp(requestDir);
        if (mappedApplet == nullptr) {
            // try static applet
            applet = staticHandler->getInstance();
        } else {
            // get a new instance from the mapped model
            applet = mappedApplet->getInstance();
            if (applet == nullptr) {
                // some unidentified error has occurred
                applet = ErrorHandler::getHandlerByStatusCode(500);
            }
        }
    }

    // run applet process
    applet->setRequest(*request);
    bool hasProcessError = false;
    // keep trying until no error occurs
    do {
        try {
            applet->process();
            hasProcessError = false;
        } catch (StaticResourceNotFound &resourceNotFoundError) {
            applet = ErrorHandler::getHandlerByStatusCode(404);
            applet->setRequest(*request);
            hasProcessError = true;
        }
    } while (hasProcessError);


    // send back response
    applet->getResponse()->send(*tcpChannel);

    returnHttpConnection(tcpChannel, request);
    cleanUpSession(applet->getResponse(), applet);
}

HttpApplet *HttpDispatcher::getMappedApp(const SString &dir) {
    auto it = requestMap.find(dir);
    if (it == requestMap.end_iterator()) {
        return nullptr;
    }
    return it->get_val();
}

void HttpDispatcher::registerApplets() {
    requestMap.put(SString("/"), new HelloApplet());
}

void HttpDispatcher::returnHttpConnection(TcpSocketChannel *tcpChannel, Request *request) {
    // connection control on keep-alive
    size_type timeOut;
    if (request) {
        timeOut = request->getKeepAliveTimeOut();
    } else {
        timeOut = 0;
    }
    if (timeOut == 0) {
        // check if channel is registered
        fdMapLock.lock();
        auto it = fd2TimeOut.find(tcpChannel->getFd());
        if (it != fd2TimeOut.end_iterator()) {
            fd2TimeOut.remove(it);
        }
        fdMapLock.unlock();
        // connection already closed
        delete tcpChannel;
        delete request;
        return;
    }
    // put into the set
    fdMapLock.lock();
    fd2TimeOut.put(tcpChannel->getFd(), {timeOut, time(nullptr)});
    fdMapLock.unlock();
    tcpChannel->invalidi();
    tcpChannel->invalido();
    delete tcpChannel;
    delete request;
}

void HttpDispatcher::cleanUpSession(Response *response, HttpApplet *applet) {
    delete response;
    applet->release();
}

void HttpDispatcher::throwErrorStatus(int status, TcpSocketChannel &tcpChannel) {

}

inline static bool rightMinusLeftLargerThanDiff(time_t &left, time_t &right, size_type diff) {
    size_type thisDiff = right - left;
    return thisDiff - diff;
}

void HttpDispatcher::CleanThread::run() {
    HashMap<int, KeepAliveInfo> newMap;
    while (true) {
        _this->fdMapLock.lock();
        for (auto it = _this->fd2TimeOut.begin_iterator(); it != _this->fd2TimeOut.end_iterator(); ++it) {
            time_t now = time(nullptr);
            if (rightMinusLeftLargerThanDiff(it->get_val().createdTime, now, it->get_val().timeOut)) {
                TcpSocketChannel tcpChannel(it->get_key());
                _this->listener->closeChannel(&tcpChannel);
            } else {
                newMap.insert(move(*it));
            }
        }
        _this->fd2TimeOut = newMap;
        _this->fdMapLock.unlock();
        Thread::sleep(Time(5));
    }
}

