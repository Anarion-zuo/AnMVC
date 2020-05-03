//
// Created by anarion on 2020/4/1.
//

#ifndef MYMVC_FILEPAYLOAD_H
#define MYMVC_FILEPAYLOAD_H

#include <io/channel/file/FileChannel.h>
#include "Payload.h"

namespace anarion {
    class FilePayload : public virtual Payload {
    protected:

        FileChannel *fileChannel;
        char *fileCache = nullptr;
        size_type fileLength = 0;
        timespec lastLoadTime = {0, 0};
        SString contentType;
        static SString defaultContentType, defaultContentEncoding;

        static HashMap<SString, SString> suffix2ContentType;


        bool needUpdate();

        void unload_impl();

    public:

        size_type length() override;

//        explicit FilePayload(SString &&dir) : fileChannel(FileChannel::open(forward<SString>(dir))) {}
        explicit FilePayload(FileChannel *filechannel) : fileChannel(filechannel) {}
        ~FilePayload() override { unload_impl(); }

        void load() override;
        bool isLoaded() override;
        void unload() override { unload_impl(); }

        void send(SocketChannel &channel) override ;

        const SString & getContentType() override { return contentType; }
        const SString &getContentEncoding() override { return defaultContentEncoding; }
        void setContentTypeBySuffix(const SString &suffix);
    };
}

#endif //MYMVC_FILEPAYLOAD_H
