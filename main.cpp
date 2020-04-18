#include <iostream>
#include "http/HttpDispatcher.h"


using namespace anarion;

int main() {
    TcpPortListener *listener = new TcpPortListener(9898);
    listener->launch();

    HttpDispatcher *dispatcher = new HttpDispatcher(listener);
    while (true) {
        dispatcher->process();
    }
    return 0;
}
