//
// Created by anarion on 2020/2/28.
//

#include "../../include/http/Request.h"
#include <parser/ListParser.h>

namespace anarion {
    static constexpr size_type indexOf(const char *p, size_type length, char c) {
        for (size_type i = 0; i < length; ++i) {
            if (p[i] == c) { return i; }
        }
        return length;
    }

    struct HttpRequestTextSyntaxError : public std::exception {
        const char *text;
        size_type length;

//        HttpRequestTextSyntaxError(const char *text, size_type length) : text(text), length(length) {}

        const char *what() const noexcept override {
            return "Failed to parse incoming HTTP request.";
        }
    };

    static ListParser *spaceSeparatedParser = new ListParser(' ');
    static MapParser *paramMapParser = new MapParser('=', '&');

    /*
     * Parse the literal part of the request
     */
    Request *parseRequest(char *data, size_type length, size_type *totalHeaderLength) {
        if (data == nullptr || length == 0) {
            return nullptr;
        }
        Request *request = new Request;

        size_type leftIndex = 0, rOffset;

        /*
         * First line of the request frame
         * GET /hello HTTP/1.1
         */
        rOffset = indexOf(data, length, '\r');
        if (rOffset == length) {
            throw HttpRequestTextSyntaxError();
        }
        if (data[rOffset + 1] != '\n') {
            throw HttpRequestTextSyntaxError();
        }
        LinkedList<SString> firstLineVars = spaceSeparatedParser->parse(data, rOffset);
        if (firstLineVars.size() != 3) {
            throw HttpRequestTextSyntaxError();
        }
        auto firstLineIt = firstLineVars.begin_iterator();
        request->method = Request::string2type(*firstLineIt);
        ++firstLineIt;
        SString dirparam = move(*firstLineIt);
//        request->dir = move(*firstLineIt);
        ++firstLineIt;
        // http version

        /*
         * Request directory&params
         */
        {
            const char *dir_str = dirparam.getArr();
            size_type dir_len = dirparam.length();

            // find ?
            size_type quOffset = indexOf(dir_str, dir_len, '?');
            request->dir = SString(const_cast<char*>(dir_str), quOffset);
            if (quOffset != dir_len) {
                request->params = paramMapParser->parse(dir_str + quOffset + 1, quOffset + 1);
            }
        }

        /*
         * Headers
         * 1) change all \n to space
         * 2) apply map parser
         */
        {
            leftIndex = rOffset + 2;
            size_type headersBeginIndex = leftIndex, headersEndIndex;
            // change all \n to space
            while (true) {
                rOffset = indexOf(data + leftIndex, length - leftIndex, '\r') + leftIndex;
                if (rOffset == length) {
                    throw HttpRequestTextSyntaxError();
                }
                if (data[rOffset + 1] != '\n') {
                    throw HttpRequestTextSyntaxError();
                }
                if (rOffset == leftIndex) {
                    headersEndIndex = leftIndex - 2;  // the last non-empty \r
                    leftIndex = rOffset + 2;
                    break;
                }
                leftIndex = rOffset + 1;
                data[leftIndex] = ' ';
                ++leftIndex;
            }
            request->headers = MapParser(':', '\r').parse(data + headersBeginIndex,
                                                          headersEndIndex - headersBeginIndex);
        }
        *totalHeaderLength = leftIndex;
        return request;
    }
}


anarion::Request * anarion::Request::parse(anarion::Buffer &data) {
    /*
     * Literal part
     */
    Request *request = nullptr;
    {
        data.rewind();
        size_type length = data.unread(), totalHeaderLength;
        char *c_data = static_cast<char *>(operator new(length));
        data.write_arr(c_data, length);
        request = parseRequest(c_data, length, &totalHeaderLength);
        operator delete(c_data, length);
        if (request == nullptr) {
            throw HttpRequestTextSyntaxError();
        }
        data.setWriteIndex(totalHeaderLength);
    }
    /*
     * Connection
     */
    {
        request->parseHeaderConnection();
        request->parseHeaderKeepAlive();
    }
    return request;
}

anarion::SString anarion::Request::GetMethodString("GET"), anarion::Request::PostMethodString("POST"), anarion::Request::PutMethodString("PUT"), anarion::Request::DeleteMethodString("DELETE"), anarion::Request::HeadMethodString("HEAD"), anarion::Request::ConnectMethodString("CONNECT"), anarion::Request::OptionsMethodString("OPTIONS"), anarion::Request::TraceMethodString("TRACE"), anarion::Request::PatchMethodString("PATCH");

anarion::RequestMethod anarion::Request::string2type(const char *str, size_type len) {
    SString input { const_cast<char*>(str), len };
    if (input == GetMethodString) { return REQUEST_GET; }
    if (input == PostMethodString) { return REQUEST_POST; }
    if (input == PutMethodString) { return REQUEST_PUT; }
    if (input == DeleteMethodString) { return REQUEST_DELETE; }
    if (input == HeadMethodString) { return REQUEST_HEAD; }
    if (input == ConnectMethodString) { return REQUEST_CONNECT; }
    if (input == OptionsMethodString) { return REQUEST_OPTIONS; }
    if (input == TraceMethodString) { return REQUEST_TRACE; }
    if (input == PatchMethodString) { return REQUEST_PATCH; }
    return REQUEST_UNSUPPORTED;
}

anarion::RequestMethod anarion::Request::string2type(const anarion::SString &string) {
    if (string == GetMethodString) { return REQUEST_GET; }
    if (string == PostMethodString) { return REQUEST_POST; }
    if (string == PutMethodString) { return REQUEST_PUT; }
    if (string == DeleteMethodString) { return REQUEST_DELETE; }
    if (string == HeadMethodString) { return REQUEST_HEAD; }
    if (string == ConnectMethodString) { return REQUEST_CONNECT; }
    if (string == OptionsMethodString) { return REQUEST_OPTIONS; }
    if (string == TraceMethodString) { return REQUEST_TRACE; }
    if (string == PatchMethodString) { return REQUEST_PATCH; }
    return REQUEST_UNSUPPORTED;
}

anarion::HashMap<anarion::SString, anarion::Request::ConnectionHeaderEnum> anarion::Request::connectionStr2Enum (
{
        { anarion::SString("keep-alive"), ConnectionKeepAlive },
        { anarion::SString("Transfer-Encoding"), ConnectionTransferEncoding },
        { anarion::SString("TE"), ConnectionTE },
        { anarion::SString("Connection"), ConnectionConnection },
        { anarion::SString("Trailer"), ConnectionTrailer },
        { anarion::SString("Upgrade"), ConnectionUpgrade },
        { anarion::SString("Proxy-Authorization"), ConnectionProxyAuthorization },
        { anarion::SString("Proxy-Authenticate"), ConnectionProxyAuthenticate },
});

void anarion::Request::parseHeaderConnection() {
    auto headerIt = headers.find(SString("Connection"));
    if (headerIt == headers.end_iterator()) {
        return ;
    }
    SString &connectionStr = headerIt->get_val();
    auto connectionIt = connectionStr2Enum.find(connectionStr);
    if (connectionIt == connectionStr2Enum.end_iterator()) {
        return ;
    }
    connectionHeader = connectionIt->get_val();
}

anarion::SString anarion::Request::KeepAliveStr("keep-alive"), anarion::Request::TimeOutStr("timeout"), anarion::Request::MaxStr("max");
anarion::MapParser * anarion::Request::keepAliveParser = new MapParser('=', ';');

anarion::size_type anarion::Request::keepAliveDefaultSeconds = 1000;

void anarion::Request::parseHeaderKeepAlive() {
    if (connectionHeader == ConnectionKeepAlive) {
        auto it = headers.find(SString("Keep-Alive"));
        keepAlive = true;
        if (it != headers.end_iterator()) {
            HashMap<SString, SString> map = keepAliveParser->parse(it->get_val());
            // timeout
            auto timeout_it = map.find(TimeOutStr);
            if (timeout_it != map.end_iterator()) {
                keepAliveTimeOut = timeout_it->get_val().toDecUnsigned();
            } else {
                keepAliveTimeOut = keepAliveDefaultSeconds;
            }
            // max
            auto max_it = map.find(MaxStr);
            if (max_it != map.end_iterator()) {
                keepAliveMax = max_it->get_val().toDecSigned();
            }
        } else {
            keepAliveTimeOut = keepAliveDefaultSeconds;
        }
    }
}

