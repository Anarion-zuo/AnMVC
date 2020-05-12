#include <iostream>
#include <logger/MvcLogger.h>
#include <http/HelloApplet.h>
#include "http/HttpDispatcher.h"


using namespace anarion;

Request *anarion::parseRequest(char *data, size_type length, size_type *totalHeaderLength);

int main() {
    TcpPortListener *listener = new TcpPortListener(8080);
    listener->launch();


    HttpDispatcher *dispatcher = new HttpDispatcher(listener);
    dispatcher->addMapping(SString("/"), new HelloApplet);
    while (true) {
        dispatcher->process();
    }
//    size_type totalHeaderLength;
//    char *cdata = "GET /html/hello.html HTTP/1.1\r\nhaha: f\r\n\r\n";
//    size_type len = strlen(cdata);
//    char *data = static_cast<char *>(operator new(len));
//    memcpy(data, cdata, len);
//    Request *request = parseRequest(data, strlen(data), &totalHeaderLength);
    return 0;
}
