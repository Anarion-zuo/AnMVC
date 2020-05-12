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

    void onGet(Request *request, Response *response) override;

public:
};
}

#endif //MYMVC_HELLOAPPLET_H
