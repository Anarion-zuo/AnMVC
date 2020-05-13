#include <iostream>
#include <logger/MvcLogger.h>
#include <http/HttpContext.h>


using namespace anarion;

int main() {
    HttpContext *context = new HttpContext(8080);
    context->execute();
//    size_type totalHeaderLength;
//    char *cdata = "GET /html/hello.html HTTP/1.1\r\nhaha: f\r\n\r\n";
//    size_type len = strlen(cdata);
//    char *data = static_cast<char *>(operator new(len));
//    memcpy(data, cdata, len);
//    Request *request = parseRequest(data, strlen(data), &totalHeaderLength);
    return 0;
}
