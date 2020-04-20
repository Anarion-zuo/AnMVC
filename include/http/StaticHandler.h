//
// Created by anarion on 4/20/20.
//

#ifndef MYMVC_STATICHANDLER_H
#define MYMVC_STATICHANDLER_H

#include "HttpApplet.h"

namespace anarion {
    class StaticHandler : public HttpApplet {
        static ItemPool<StaticHandler> pool;
    protected:
        void onGet() override;
        HttpApplet *getInstance() override;
    };
}

#endif //MYMVC_STATICHANDLER_H
