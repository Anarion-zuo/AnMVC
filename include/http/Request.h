//
// Created by anarion on 2020/2/28.
//

#ifndef MYMVC_REQUEST_H
#define MYMVC_REQUEST_H

#include <container/SString.h>
#include <container/Map/HashMap.hpp>
#include <io/buffer/Buffer.h>
#include <io/channel/network/TcpSocketChannel.h>
#include <parser/MapParser.h>

namespace anarion {

    enum RequestMethod { REQUEST_GET, REQUEST_POST, REQUEST_PUT, REQUEST_DELETE, REQUEST_HEAD, REQUEST_CONNECT, REQUEST_OPTIONS, REQUEST_TRACE, REQUEST_PATCH, REQUEST_UNSUPPORTED};

    class HttpDispatcher;
    class Request {
        friend class HttpDispatcher;
        friend Request *parseRequest(char *data, size_type length, size_type *totalHeaderLength);
    protected:

        // basic attributes

        SString dir;
        HashMap<SString, SString> headers, params;
        Buffer data{ 1024 };

        /*
         * Server parameters
         * for the applet to hack down into Connection details
         */
        HttpDispatcher *dispatcher = nullptr;
        TcpSocketChannel *tcpChannel = nullptr;

        /*
         * Request Methods
         * static strings for swift comparison
         */
        static SString GetMethodString, PostMethodString, PutMethodString, DeleteMethodString, HeadMethodString, ConnectMethodString, OptionsMethodString, TraceMethodString, PatchMethodString;
        enum RequestMethod method;   // shown in enum RequestType
        static anarion::RequestMethod string2type(const char *str, size_type len);
        static RequestMethod string2type(const SString &string);

        // The class must not be instantiated explicitly with a constructor
        Request() = default;

        // special attributes

        /*
         * The following members handle special features in http.
         * Some of the common headers must be parsed.
         */

        /*
         * Connection
         * Header attribute connection, specified to show the purpose of this connection.
         */
        enum ConnectionHeaderEnum { ConnectionKeepAlive, ConnectionTransferEncoding, ConnectionTE, ConnectionConnection, ConnectionTrailer, ConnectionUpgrade, ConnectionProxyAuthorization, ConnectionProxyAuthenticate, ConnectionClose };
        static HashMap<SString, ConnectionHeaderEnum> connectionStr2Enum;
        void parseHeaderConnection();
        ConnectionHeaderEnum connectionHeader = ConnectionClose;  // default closed

        /*
         * Keep Alive
         * Header attribute, specified to maintain connection, not releasing it after a single session.
         *
         * This handling must happen after handling Connection
         */
        static SString KeepAliveStr, TimeOutStr, MaxStr;
        static MapParser *keepAliveParser;
        bool keepAlive = false;  // default false, change if otherwise
        size_type keepAliveTimeOut = 0;
        size_type keepAliveMax = 0;
        static size_type keepAliveDefaultSeconds;
        void parseHeaderKeepAlive();

    public:

        Request(const Request &rhs) : dir(rhs.dir), headers(rhs.headers), params(rhs.params), data(rhs.data) {}
        Request(Request &&rhs) noexcept : dir(move(rhs.dir)), headers(move(rhs.headers)), params(move(rhs.params)) {}

        static Request * parse(Buffer &data);

        SString &getHeaders(const SString &key) { return headers.get(key); }
        SString &getParams(const SString &key) { return params.get(key); }
        constexpr Buffer &getData() { return data; }
        constexpr SString &getDir() { return dir; }
        constexpr RequestMethod getMethod() const { return method; }
        constexpr HttpDispatcher *getDispatcher() const { return dispatcher; }
        constexpr TcpSocketChannel *getSocket() const { return tcpChannel; }

        /*
         * Features in http
         */
        constexpr size_type getKeepAliveTimeOut() const {
            if (!keepAlive) {
                return 0;
            }
            return keepAliveTimeOut;
        }

    };
}

#endif //MYMVC_REQUEST_H
