//
// Created by anarion on 4/20/20.
//

#ifndef MYMVC_STATICHANDLER_H
#define MYMVC_STATICHANDLER_H

#include "HttpApplet.h"

namespace anarion {
    class StaticHandler : public HttpApplet {
    protected:
        void onGet(Request *request, Response *response) override;

    public:
    };

struct StaticResourceNotFound : public std::exception {
    const char *what() const noexcept override;
};
}

#endif //MYMVC_STATICHANDLER_H
