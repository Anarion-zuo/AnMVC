//
// Created by anarion on 2020/2/29.
//

#include "http/Response.h"

anarion::HashMap<int, anarion::SString> anarion::Response::status2desc (
        {
                {100, SString("Continue")},
                { 101, SString("Switching Protocols") },
                { 200, SString("OK") },
                { 201, SString("Created") },
                { 202, SString("Accepted") },
                { 203, SString("Non-Authoritative Information") },
                { 204, SString("No Content") },
                { 205, SString("Reset Content") },
                { 206, SString("Partial Content") },
                { 300, SString("Multiple Choices") },
                { 301, SString("Moved Permanently") },
                { 302, SString("Found") },
                { 303, SString("See Other") },
                { 304, SString("Not Modified") },
                { 305, SString("Use Proxy") },
                { 306, SString("Unused") },
                { 307, SString("Temporary Redirect") },
                { 400, SString("Bad Request") },
                { 401, SString("Unauthorized") },
                { 402, SString("Payment Required") },
                { 403, SString("Forbidden") },
                { 404, SString("Not Found") },
                { 405, SString("Method Not Allowed") },
                { 406, SString("Not Acceptable") },
                { 407, SString("Proxy Authentication Required") },
                { 408, SString("Request Time-out") },
                { 409, SString("Conflict") },
                { 410, SString("Gone") },
                { 411, SString("Length Required") },
                { 412, SString("Precondition Failed") },
                { 413, SString("Request Entity Too Large") },
                { 414, SString("Request-URI Too Large") },
                { 415, SString("Unsupported Media Type") },
                { 416, SString("Requested range not satisfiable") },
                { 417, SString("Expectation Failed") },
                { 500, SString("Internal Server Error") },
                { 501, SString("Not Implemented") },
                { 502, SString("Bad Gateway") },
                { 503, SString("Service Unavailable") },
                { 504, SString("Gateway Time-out") },
                { 505, SString("HTTP Version not supported") },
        }
);
anarion::HashMap<int, anarion::SString> anarion::Response::status2num (
{
        { 100, SString("100") },
        { 101, SString("101") },
        { 200, SString("200") },
        { 201, SString("201") },
        { 202, SString("202") },
        { 203, SString("203") },
        { 204, SString("204") },
        { 205, SString("205") },
        { 206, SString("206") },
        { 300, SString("300") },
        { 301, SString("301") },
        { 302, SString("302") },
        { 303, SString("303") },
        { 304, SString("304") },
        { 305, SString("305") },
        { 306, SString("306") },
        { 307, SString("307") },
        { 400, SString("400") },
        { 401, SString("401") },
        { 402, SString("402") },
        { 403, SString("403") },
        { 404, SString("404") },
        { 405, SString("405") },
        { 406, SString("406") },
        { 407, SString("407") },
        { 408, SString("408") },
        { 409, SString("409") },
        { 410, SString("410") },
        { 411, SString("411") },
        { 412, SString("412") },
        { 413, SString("413") },
        { 414, SString("414") },
        { 415, SString("415") },
        { 416, SString("416") },
        { 417, SString("417") },
        { 500, SString("500") },
        { 501, SString("501") },
        { 502, SString("502") },
        { 503, SString("503") },
        { 504, SString("504") },
        { 505, SString("505") },
}
);

anarion::SString anarion::Response::versionStr {"HTTP/1.1"};  // HTTP/1.1

void anarion::Response::send(anarion::SocketChannel &channel) {
    if (payload) {
        if (!payload->isLoaded()) {
            payload->load();
        }
    }
    // setup header attributes
    addContentType();
    addContentLength();
    addContentEncoding();
    addDate();

    // allocate contiguous space
    size_type header_len = versionStr.size();
    header_len += 3;   // status code
    header_len += 2;   // spaces
    SString &desc = status2desc.get(status);
    header_len += desc.size();  // status description
    header_len += 2;  // \r\n
    for (auto it = headers.begin_iterator(); it != headers.end_iterator(); ++it) {
        header_len += it->get_key().size() + it->get_val().size() + 2 + 2;
        //                 key                    val              :    \r\n
    }
    header_len += 2;  // empty line
    char *p = static_cast<char *>(operator new(header_len));

    // copy header
    versionStr.copy_cstr(p);
    p += versionStr.size();
    *p = ' ';
    ++p;
    status2num.get(status).copy_cstr(p);
    p += 3;
    *p = ' ';
    ++p;
    desc.copy_cstr(p);
    p += desc.size();
    *p = '\r'; ++p; *p = '\n'; ++p;
    for (auto it = headers.begin_iterator(); it != headers.end_iterator(); ++it) {
        it->get_key().copy_cstr(p);
        p += it->get_key().size();
        *p = ':'; ++p; *p = ' '; ++p;
        it->get_val().copy_cstr(p);
        p += it->get_val().size();
        *p = '\r'; ++p; *p = '\n'; ++p;
    }
    *p = '\r'; ++p; *p = '\n'; ++p;

    // send header
    p -= header_len;
    channel.in(p, header_len);
    operator delete (p, header_len);
    // send payload
    if (payload) {
        payload->send(channel);
    }
}

void anarion::Response::addDate() {
    // set current date, not file last modified
    ::time_t systime = ::time(nullptr);
    char str[50];
    ::strftime(str, 100, "%a, %d, %b %Y %T GMT", ::localtime(&systime));
    // Mon, 27 Jan 2020 07:18:33 GMT
    size_type len = ::strlen(str);
    addHeader(SString("Date", 4), SString(str, len));
}

void anarion::Response::addContentType() {
    if (payload) {
        addHeader(SString("Content-Type"), SString(payload->getContentType()));
    }
}

void anarion::Response::addContentLength() {
    if (payload) {
        addHeader(SString("Content-Length"), SString::parseDec(payload->length()));
    } else {
        addHeader(SString("Content-Length"), SString("0"));
    }
}

void anarion::Response::addContentEncoding() {
    if (payload) {
        const SString &encoding = payload->getContentEncoding();
        if (encoding.size()) {
            addHeader(SString("Content-Encoding"), SString(payload->getContentEncoding()));
        }
    }
}
