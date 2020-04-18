// Created by anarion on 2020/4/2.
//

#ifndef MYMVC_CSSPAYLOAD_H
#define MYMVC_CSSPAYLOAD_H

#include "FilePayload.h"

namespace anarion {
    class CssPayload : public virtual FilePayload {
    protected:
        static SString defaultContentType;
    public:
        explicit CssPayload(SString &&dir);

        const SString &getContentType() override;
    };
}

#endif //MYMVC_CSSPAYLOAD_H
