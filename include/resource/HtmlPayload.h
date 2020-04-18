//
// Created by anarion on 2020/4/2.
//

#ifndef MYMVC_HTMLPAYLOAD_H
#define MYMVC_HTMLPAYLOAD_H

#include "FilePayload.h"

namespace anarion {
    class HtmlPayload : public virtual FilePayload {
    protected:
        static SString defaultContentType;

    public:
        explicit HtmlPayload(SString &&dir);

        const SString &getContentType() override;
    };
}

#endif //MYMVC_HTMLPAYLOAD_H
