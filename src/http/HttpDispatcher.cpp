//
// Created by anarion on 2020/3/31.
//

#include <http/HelloApplet.h>
#include "http/HttpDispatcher.h"

using namespace anarion;


void anarion::HttpDispatcher::process() {
    // obtain channel
    TcpSocketChannel *tcpChannel = listener->pollQueue();
    HttpChannel *httpChannel;
    httpChannel = new HttpChannel(move(*tcpChannel));
    tcpChannel->invalidi();
    tcpChannel->invalido();
    delete tcpChannel;

    // read and parse data
    Buffer channelData = httpChannel->out();
    if (channelData.empty()) {
        // TODO connection closed
        returnConnection(httpChannel, nullptr);
        // no cleanup required
        return;
    }
    Request * request = Request::parse(channelData);
    httpChannel->loadRequest(request);

    // map to applet
    SString &requestDir = request->getDir();
    HttpApplet *applet = getMappedApp(requestDir);
    if (applet == nullptr) {
        // TODO no request mapping exists
        Response notFoundResponse (404);
        notFoundResponse.send(*httpChannel);
        returnConnection(httpChannel, request);
        // no cleanup required
        return;
    }

    // run applet process
    HttpApplet *instance = applet->getInstance();
    instance->setRequest(*request);
    instance->process();

    // send back response
    instance->getResponse()->send(*httpChannel);

    returnConnection(httpChannel, request);
    cleanUpSession(instance->getResponse(), instance);
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

void HttpDispatcher::returnConnection(HttpChannel *httpChannel, Request *request) {
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
        auto it = fd2TimeOut.find(httpChannel->getFd());
        if (it != fd2TimeOut.end_iterator()) {
            fd2TimeOut.remove(*it);
        }
        fdMapLock.unlock();
        // connection already closed
        delete httpChannel;
        return;
    }
    // put into the set
    fdMapLock.lock();
    fd2TimeOut.put(httpChannel->getFd(), {timeOut, time(nullptr)});
    fdMapLock.unlock();
    httpChannel->invalidi();
    httpChannel->invalido();
    delete httpChannel;
    delete request;
}

void HttpDispatcher::cleanUpSession(Response *response, HttpApplet *applet) {
    delete response;
    applet->release();
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
        sleep(5);
    }
}

