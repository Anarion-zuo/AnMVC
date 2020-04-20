//
// Created by anarion on 4/20/20.
//

#ifndef MYMVC_TEXTPAYLOAD_H
#define MYMVC_TEXTPAYLOAD_H

#include "Payload.h"

namespace anarion {
    class TextPayload : virtual public Payload {
    protected:
        SString text;

        static SString defaultType, defaultEncoding;
    public:
        const SString &getContentEncoding() override;

    public:
        explicit TextPayload(SString text);

        size_type length() override;
        void send(SocketChannel &channel) override;
        bool isLoaded() override;
        const SString &getContentType() override;
        void load() override {}
    };


}

#endif //MYMVC_TEXTPAYLOAD_H
