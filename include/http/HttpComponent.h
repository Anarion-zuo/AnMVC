//
// Created by anarion on 5/14/20.
//

#ifndef MYMVC_HTTPCOMPONENT_H
#define MYMVC_HTTPCOMPONENT_H

namespace anarion {
    class HttpContext;
    class HttpComponent {
    private:
        HttpContext *context;
    public:
        explicit HttpComponent(HttpContext *context) : context(context) {}
        HttpContext &getContext() { return *context; }
    };
}

#endif //MYMVC_HTTPCOMPONENT_H
