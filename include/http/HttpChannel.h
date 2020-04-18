//
// Created by anarion on 2020/4/2.
//

#ifndef MYMVC_HTTPCHANNEL_H
#define MYMVC_HTTPCHANNEL_H

#include <io/channel/network/TcpSocketChannel.h>
#include <concurrent/container/Queue/BlockQueue.hpp>
#include <concurrent/base/Thread.h>
#include <container/Set/HashSet.hpp>
#include <container/Map/HashMap.hpp>
#include <listener/TcpPortListener.h>
#include "Request.h"

namespace anarion {
    class HttpChannel : public TcpSocketChannel {
        friend class HttpAliveChannelQueue;
    protected:
        size_type timeOut = 0;
        time_t establishedTime;  // initialized by constructor, in millisecond

    public:
        explicit HttpChannel(int cfd) : InChannel(true), OutChannel(true), SocketChannel(cfd), TcpSocketChannel(cfd), establishedTime(time(nullptr)) {}
        explicit HttpChannel(TcpSocketChannel &&tcpChannel) : InChannel(forward<TcpSocketChannel>(tcpChannel)), OutChannel(forward<TcpSocketChannel>(tcpChannel)), SocketChannel(forward<TcpSocketChannel>(tcpChannel)), TcpSocketChannel(forward<TcpSocketChannel>(tcpChannel)), establishedTime(time(nullptr)) {}

        void loadRequest(Request *request);

        bool isTimeOut() const {
            time_t now = time(nullptr);
            time_t diff = now - establishedTime;
            return (diff * 1000) > establishedTime;   // must change time unit millisecond->second
        }
    };

    class HttpAliveChannelQueue {
    protected:

        Mutex lock, waitLock;
        CondVar cond;
        TcpPortListener *listener = nullptr;
        LinkedList<HttpChannel*> deadList, aliveList;

        struct CleaningThread : public Thread {

            HttpAliveChannelQueue *_this = nullptr;  // must be injected, or addressing nullptr

//            void run() override;
        };

        CleaningThread cleaningThread;

    public:
        explicit HttpAliveChannelQueue(TcpPortListener *listener) : listener(listener), lock(), waitLock(), cond(waitLock) { cleaningThread._this = this; cleaningThread.start(); }

        void addChannel(HttpChannel *httpChannel);
    };

}

#endif //MYMVC_HTTPCHANNEL_H
