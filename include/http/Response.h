//
// Created by anarion on 2020/2/29.
//

#ifndef MYMVC_RESPONSE_H
#define MYMVC_RESPONSE_H

#include <container/Map/HashMap.hpp>
#include <container/SString.h>
#include <io/channel/network/SocketChannel.h>
#include <resource/Payload.h>

/*
 * This is a class for http response
 * It does not contain data field, only the header part
 * Any appending data must be sent just after the response class
 */

namespace anarion {
    class Response {
    protected:
        HashMap<SString, SString> headers;
        int status;
        Payload *payload = nullptr;

        static HashMap<int, SString> status2desc, status2num;
        static SString versionStr;  // HTTP/1.1

    public:

        explicit Response(int status = 200) : status(status) {}

        SString &getHeader(const SString &key) { return headers.get(key); }
        constexpr int getStatus() const { return status; }
        constexpr void setStatus(int n) { status = n; }

        constexpr void setPayload(Payload *p) { payload = p; }

        void addHeader(SString &&key, SString &&val) { headers.put(forward<SString>(key), forward<SString>(val)); }
        void addDate();
        void addContentLength();
        void addContentType();
        void addContentEncoding();

        void send(SocketChannel &channel);
    };
}

#endif //MYMVC_RESPONSE_H
