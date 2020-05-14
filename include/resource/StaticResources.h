//
// Created by anarion on 4/15/20.
//

#ifndef MYMVC_STATICRESOURCES_H
#define MYMVC_STATICRESOURCES_H

#include <io/channel/file/Directory.h>
#include "FilePayload.h"
#include "LRUPayloadManager.h"
#include <resource/FixedSizeCache.hpp>

namespace anarion {
    class StaticResources {
    protected:

//        static SString htmlString, cssString, jsString, imgString;
        static HashSet<SString> imageFormatSet;

        Directory directory;
//        Directory *htmlDirectory, *cssDirectory, *jsDirectory, *imgDirectory;
        HashMap<SString, Directory*> name2dir;
        Directory *getChildDirectoryByName(const SString &name);

        HashMap<SString, FilePayload*> dir2payload;

        LRUPayloadManager *manager = new LRUPayloadManager(100);

        void loadPayload(Payload *payload);

    public:
        explicit StaticResources(SString &&dir);

        ~StaticResources() {
            delete manager;
        }

        FileChannel * getFile(const SString &relDir);
        Payload *getPayload(const SString &relDir);

    };
}

#endif //MYMVC_STATICRESOURCES_H
