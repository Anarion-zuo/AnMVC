//
// Created by anarion on 2020/3/31.
//

#ifndef MYMVC_HELLOAPPLET_H
#define MYMVC_HELLOAPPLET_H

#include "HttpApplet.h"

namespace anarion {
    class CompressedPayload;
class HelloApplet : public virtual HttpApplet {
protected:

    static ItemPool<HelloApplet> pool;

    void onGet() override;

public:
    HttpApplet *getInstance() override;

    void release() override;
};
}

#endif //MYMVC_HELLOAPPLET_H
