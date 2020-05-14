//
// Created by anarion on 2020/3/31.
//

#ifndef MYMVC_HTTPDISPATCHER_H
#define MYMVC_HTTPDISPATCHER_H

#include "../../include/listener/TcpPortListener.h"
#include <container/Map/HashMap.hpp>
#include <container/SString.h>
#include "HttpApplet.h"
#include "HttpChannel.h"
#include "StaticHandler.h"
#include "HttpComponent.h"
/*
 * The dispatcher collects connections (Channel) from specified listener (listener) and forwards them to registered HttpApplet instances.
 * The listener is pre-specified at construct time.
 * The listener must be launch beforehand (call launch).
 *
 * Recommend using singleton...
 */

namespace anarion {
    class HttpDispatcher : public HttpComponent {
    protected:
        TcpPortListener *listener;
        HashMap<SString, HttpApplet*> requestMap;


        HttpApplet *getMappedApp(const SString &dir);
        void registerApplets();
        void returnHttpConnection(TcpSocketChannel *tcpChannel, Request *request);
        void cleanUpSession(Response *response, HttpApplet *applet);

        struct CleanThread : public Thread {
            HttpDispatcher *_this;
            void run() override;
        } connectionCleanThread;

        /*
         * Keep Alive members
         */
        struct KeepAliveInfo {
            size_type timeOut;
            time_t createdTime;

            KeepAliveInfo(size_type timeOut, time_t createdTime) : timeOut(timeOut), createdTime(createdTime) {}
        };
        HashMap<int, KeepAliveInfo> fd2TimeOut;
        Mutex fdMapLock;

    public:
        explicit HttpDispatcher(HttpContext *context, TcpPortListener *listener1) : HttpComponent(context), listener(listener1), fdMapLock() {
            registerApplets();
            connectionCleanThread._this = this;
            connectionCleanThread.start();
        }

        /*
         * Call repeatedly, each time processing a single connection from listener.
         */
        void process();
        void addMapping(SString &&dir, HttpApplet *applet);

        void throwErrorStatus(int status, TcpSocketChannel &tcpChannel);
    };
}

#endif //MYMVC_HTTPDISPATCHER_H
