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

    static constexpr size_type skipChar(const char *expr, size_type length, char c) {
        for (size_type i = 0; i < length; ++i) {
            if (expr[i] != c) { return i; }
        }
        return length;
    }

    static constexpr size_type skipCharBack(const char *expr, size_type length, char c) {
        for (size_type i = 0; i < length; ++i) {
            if (expr[length - i] != c) { return i; }
        }
        return length;
    }



    static ListParser *spaceSeparatedParser = new ListParser(' ');
    static MapParser *paramMapParser = new MapParser('=', '&');

    /*
     * Parse the literal part of the request
     */
#define throw_parse_error { delete request; throw HttpRequestTextSyntaxError(); }
#define move_offsets(lineLength, offset) {\
    \
}

    Request *parseRequest(char *data, size_type length, size_type *totalHeaderLength) {
        if (data == nullptr || length == 0) {
            return nullptr;
        }
        Request *request = new Request;

        size_type leftIndex = 0, spaceOffset, unParsedLength = length;

        /*
         * Parse the first line
         */
        size_type firstLineOffset = indexOf(data, length, '\r');
        // obvious error at the end of the line
        if (firstLineOffset == length) throw_parse_error
        if (data[firstLineOffset + 1] != '\n') throw_parse_error
        size_type unParsedFirstLine = firstLineOffset;
        // request method
        size_type methodOffset = indexOf(data, firstLineOffset, ' ');
        if (methodOffset == firstLineOffset) throw_parse_error
        request->method = Request::string2type(data, methodOffset);
        leftIndex += methodOffset;
        unParsedFirstLine -= methodOffset;
        spaceOffset = skipChar(data + leftIndex, unParsedFirstLine, ' ');
        if (spaceOffset == unParsedFirstLine) throw_parse_error
        leftIndex += spaceOffset;
        unParsedFirstLine -= spaceOffset;
        // directory & attrs
        size_type dirOffset = indexOf(data + leftIndex, unParsedFirstLine, ' '), dirLeft = leftIndex;
        if (dirOffset == unParsedFirstLine) throw_parse_error
        leftIndex += dirOffset;
        unParsedFirstLine -= dirOffset;
        spaceOffset = skipChar(data + leftIndex, unParsedFirstLine, ' ');
        if (spaceOffset == unParsedFirstLine) throw_parse_error
        leftIndex += spaceOffset;
        unParsedFirstLine -= spaceOffset;
        // HTTP version
        if (firstLineOffset - leftIndex != unParsedFirstLine) throw_parse_error

        // dir & attr
        size_type dirLength = indexOf(data + dirLeft, dirOffset, '?');
        request->dir = SString::move(data + dirLeft, dirLength);
        dirLeft += dirLength + 1;
        dirOffset -= dirLength + 1;
        request->params = paramMapParser->parse(data + dirLeft, dirOffset);

        // prepare for the headers
        leftIndex += firstLineOffset + 2;
        unParsedLength -= firstLineOffset + 2;

        /*
         * headers
         */
        while (true) {
            size_type lineLength = indexOf(data + leftIndex, unParsedLength, '\r');
            if (lineLength == unParsedLength) throw_parse_error
            if (data[leftIndex + lineLength + 1] != '\n') throw_parse_error
        }

        return request;
    }

# define request_buffer_length 4096

    Request *parseFromChannel(SocketChannel &channel) {

        Request *request = new Request;

        char buffer[request_buffer_length];
        size_type firstlineLength = channel.outUntil(buffer, request_buffer_length, '\r'), leftIndex = 0, spaceOffset;
        char oneC;
        channel.out(&oneC, 1);
        if (oneC != '\n') throw_parse_error

        /*
         * First line
         */
        leftIndex = 0;
        // request method
        size_type methodOffset = indexOf(buffer, firstlineLength, ' ');
        if (methodOffset == firstlineLength) throw_parse_error
        request->method = Request::string2type(buffer, methodOffset);
        leftIndex += methodOffset;
        spaceOffset = skipChar(buffer + leftIndex, firstlineLength, ' ');
        if (spaceOffset == firstlineLength) throw_parse_error
        leftIndex += spaceOffset;
        firstlineLength -= spaceOffset;
        // dir params
        size_type dirOffset = indexOf(buffer + leftIndex, firstlineLength, ' '), dirLeft = leftIndex;
        if (dirOffset == firstlineLength) throw_parse_error
        leftIndex += dirOffset;
        firstlineLength -= dirOffset;
        spaceOffset = skipChar(buffer + leftIndex, firstlineLength, ' ');
        if (spaceOffset == firstlineLength) throw_parse_error
        leftIndex += spaceOffset;
        firstlineLength -= spaceOffset;
        // HTTP version
        // ...
        // parse dir & param
        size_type dirLength = indexOf(buffer + dirLeft, dirOffset, '?');
        request->dir = SString(buffer + dirLeft, dirLength);
        if (dirLength < dirOffset) {
            dirLeft += dirLength + 1;
            dirOffset -= dirLength + 1;
            request->params = paramMapParser->parse(buffer + dirLeft, dirOffset);
        }

        /*
         * Headers
         */
        while (true) {
            size_type lineLength = channel.outUntil(buffer, request_buffer_length, '\r');
            channel.out(&oneC, 1);
            if (oneC != '\n') throw_parse_error
            if (lineLength == 0) {
                break;
            }
            leftIndex = 0;
            size_type keyLength = indexOf(buffer, lineLength, ':');
            if (keyLength == lineLength) throw_parse_error
            SString key { buffer, keyLength };
            leftIndex += keyLength + 1;
            lineLength -= keyLength + 1;
            spaceOffset = skipChar(buffer + leftIndex, lineLength, ' ');
            if (spaceOffset == lineLength) {
                request->headers.put(move(key), SString());
            } else {
                leftIndex += spaceOffset;
                lineLength -= spaceOffset;
                request->headers.put(move(key), SString(buffer + leftIndex, lineLength));
            }
        }
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

anarion::Request *anarion::Request::parse(anarion::SocketChannel &channel) {
    /*
     * Literal part
     */
    Request *request = nullptr;
    {
        request = parseFromChannel(channel);
        if (request == nullptr) {
            throw HttpRequestTextSyntaxError();
        }
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


