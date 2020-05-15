//
// Created by anarion on 2020/3/31.
//

#ifndef MYMVC_TCPPORTLISTENER_H
#define MYMVC_TCPPORTLISTENER_H

#include "StreamListener.h"
#include <io/channel/network/TcpServerSocketChannel.h>
#include <sys/epoll.h>
#include <concurrent/container/Queue/BlockQueue.hpp>
#include <concurrent/base/Thread.h>
#include <http/Request.h>

namespace anarion {
    class TcpPortListener : public virtual StreamListener {

        struct LaunchRoutine : public virtual Thread {
            TcpPortListener *listener;

            explicit LaunchRoutine(anarion::TcpPortListener *p) : listener(p) {}
            void run() override {
                listener->listen();
            }
        };

    protected:
        TcpServerSocketChannel server;   // server socket

        // epoll
        int epfd;
        epoll_event *events = nullptr;
        LaunchRoutine routine;

        // ready queue
        BlockQueue<TcpSocketChannel*> readQueue;

        // connection info
        HashMap<int, HostInfo*> infoMap;
        Mutex infoMapLock;

        bool inInfoMap(int fd);
        void putInfoMap(int fd, const HostInfo &info);
    public:
        explicit TcpPortListener(in_port_t portNum) : server(portNum), routine(nullptr) {
            routine.listener = this;
            server.setNonBlock();
        }

        void bind(in_port_t portNum) { server.bind(portNum); }
        void listen();

        void launch();

        TcpSocketChannel *pollQueue();

        void returnConnection(anarion::TcpSocketChannel *channel, Request *request);

        void closeChannel(TcpSocketChannel *channel);

        // connection info
        void removeInfoMap(int fd);
        HostInfo * getHostInfoByFd(int fd);
    };
}

#endif //MYMVC_TCPPORTLISTENER_H
