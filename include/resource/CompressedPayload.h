//
// Created by anarion on 2020/4/2.
//

#ifndef MYMVC_COMPRESSEDPAYLOAD_H
#define MYMVC_COMPRESSEDPAYLOAD_H

#include "FilePayload.h"

namespace anarion {
    class CompressedPayload : public virtual FilePayload {
    protected:
        size_type compressedLength = 0, compressedSpaceLength = 0;
        char *compressedCache = nullptr;

        static SString defaultContentEncoding;

        void unload_impl();

    public:

        explicit CompressedPayload(FileChannel *fileChannel);
        ~CompressedPayload() override { unload_impl(); }

        size_type length();
        void load() override;
        void unload() override { unload_impl(); }
        bool isLoaded() override;
        void send(SocketChannel &channel) override;

        const SString &getContentEncoding() override;
    };
}

#endif //MYMVC_COMPRESSEDPAYLOAD_H
