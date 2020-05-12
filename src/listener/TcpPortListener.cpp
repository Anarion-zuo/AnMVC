//
// Created by anarion on 2020/3/31.
//

#include "listener/TcpPortListener.h"
#include <io/channel/network/TcpSocketChannel.h>
#include <concurrent/base/Thread.h>
#include <io/base/io-exceptions.h>
#include <logger/MvcLogger.h>

namespace anarion {
    class TcpListenerLoggerInfo : public MvcLoggerInfo {
    public:
        SString serializeHostInfo(const HostInfo &info) {

        }
    };
}
static void addEvent(int epfd, int fd, int state) {
    epoll_event ev;
    memset(&ev, 0, sizeof(epoll_event));
    ev.events = state;
    ev.data.fd = fd;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    if (ret < 0) { perror("sth wrong:");
        exit(1); }
}

static void delEvent(int epfd, int fd, int state) {
    epoll_event ev;
    memset(&ev, 0, sizeof(epoll_event));
    ev.events = state;
    ev.data.fd = fd;
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
    if (ret < 0) {
        perror("sth wrong:");
        exit(1);
    }
}

void anarion::TcpPortListener::listen() {
    int backlog = 128;
    // init epoll
    epfd = ::epoll_create(backlog);  // allocate a file descriptor
    events = static_cast<epoll_event *>( operator new( backlog * sizeof(epoll_event) ) );  // allocate space for events
    memset(events, 0, backlog * sizeof(epoll_event));
    addEvent(epfd, server.getFd(), EPOLLIN | EPOLLET);  // put server socket onto the epoll tree
    int ret, fd, lfd = server.getFd();
    // initiate listen
    server.listen(backlog);
//    MvcLogger::getInstance().addInfo(new MvcLoggerInfo(SString("Epoll reactor initialized.")));
    // roll
    while (true) {
        ret = epoll_wait(epfd, events, backlog, 0);   // non block
        if (ret < 0) {
//            getLogger().addInfo(new MvcLoggerInfo(SString("Epoll wait failed. Forced to quit.")));
            perror("sth wrong:");
            exit(1);
        }
        // traverse array
        for (int index = 0; index < ret; ++index) {
            fd = events[index].data.fd;
            if ((events[index].events & EPOLLERR)) {
//                getLogger().addInfo(new MvcLoggerInfo(SString("Epoll event contains an error.")));
//                throw ListenException();
            }
            if ((events[index].events & EPOLLIN) == 0) {
                continue;
            }
            if (fd == lfd) {
                // accept
                sockaddr_in client_addr;
                socklen_t addrlen = sizeof(sockaddr_in);
                int cfd = ::accept(lfd, reinterpret_cast<sockaddr *>(&client_addr), &addrlen);
                if (cfd == -1) {
                    throw AcceptException();
                }
                HostInfo info(client_addr);
//                getLogger().addInfo();
                addEvent(epfd, cfd, EPOLLIN | EPOLLET);
                continue;
            }
            // read
            else {
                TcpSocketChannel *client = new TcpSocketChannel(fd);
                readQueue.push(client);
            }
        }
    }
}

anarion::TcpSocketChannel *anarion::TcpPortListener::pollQueue() {
    return readQueue.pop();
}

void anarion::TcpPortListener::launch() {
    routine.start();
}

void anarion::TcpPortListener::returnConnection(anarion::TcpSocketChannel *channel, Request *request) {

}

void anarion::TcpPortListener::closeChannel(TcpSocketChannel *channel) {
    delEvent(epfd, channel->getFd(), EPOLLIN);
    channel->close();
}
