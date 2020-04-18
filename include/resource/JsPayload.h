//
// Created by anarion on 2020/4/2.
//

#ifndef MYMVC_JSPAYLOAD_H
#define MYMVC_JSPAYLOAD_H

#include <container/SString.h>
#include "FilePayload.h"

namespace anarion {
    class JsPayload : public virtual FilePayload {
    protected:
        static SString defaultContentType;
    public:
        explicit JsPayload(SString &&dir);

        const SString &getContentType() override;
    };
}

#endif //MYMVC_JSPAYLOAD_H
