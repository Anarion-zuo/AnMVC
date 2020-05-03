//
// Created by anarion on 2020/4/1.
//

#ifndef MYMVC_PAYLOAD_H
#define MYMVC_PAYLOAD_H

#include <io/channel/network/SocketChannel.h>
#include <container/SString.h>

namespace anarion {
    class Payload {
    public:

        virtual ~Payload() {}

        virtual void send(SocketChannel &channel) = 0;
        virtual size_type length() = 0;
        virtual void load() = 0;
        virtual void unload() = 0;
        virtual bool isLoaded() = 0;
        virtual const SString & getContentType() = 0;
        virtual const SString & getContentEncoding() = 0;
    };
}

#endif //MYMVC_PAYLOAD_H
