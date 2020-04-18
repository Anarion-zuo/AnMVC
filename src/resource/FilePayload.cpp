//
// Created by anarion on 2020/4/1.
//

#include "resource/FilePayload.h"

anarion::SString anarion::FilePayload::defaultContentType("text/plain"), anarion::FilePayload::defaultContentEncoding;

anarion::HashMap<anarion::SString, anarion::SString> anarion::FilePayload::suffix2ContentType({
    {SString("html"), SString("text/html")},
    {SString("css"), SString("text/css")},
    {SString("js"), SString("text/javascript")},
    {SString("js"), SString("text/javascript")},

    {SString("gif"), SString("image/gif")},
    {SString("jpg"), SString("image/jpeg")},
    {SString("jpeg"), SString("image/jpeg")},
    {SString("png"), SString("image/png")},
    {SString("webp"), SString("image/webp")},
});

void anarion::FilePayload::send(anarion::SocketChannel &channel) {
    if (needUpdate()) {
        load();
    }
    channel.in(fileCache, fileLength);
}

anarion::size_type anarion::FilePayload::length() {
    return fileLength;
}

void anarion::FilePayload::load() {
    size_type len = fileChannel->size();
    if (fileCache) {  // delete if exists
        operator delete (fileCache, fileLength);
    }
    fileCache = static_cast<char *>(operator new(len));
    fileChannel->rewind();
    fileChannel->out(fileCache, len);
    fileLength = len;
    clock_gettime(CLOCK_REALTIME, &lastLoadTime);
}

bool anarion::FilePayload::needUpdate() {
    return fileChannel->modifiedLaterThan(lastLoadTime);
}

bool anarion::FilePayload::isLoaded() {
    return fileCache;
}

void anarion::FilePayload::setContentTypeBySuffix(const anarion::SString &suffix) {
    auto it = suffix2ContentType.find(suffix);
    if (it == suffix2ContentType.end_iterator()) {
        contentType = defaultContentType;
        return;
    }
    contentType = it->get_val();
}

