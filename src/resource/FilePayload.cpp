//
// Created by anarion on 2020/4/1.
//

#include "resource/FilePayload.h"

anarion::SString anarion::FilePayload::defaultContentType("text/plain"), anarion::FilePayload::defaultContentEncoding;

anarion::HashMap<anarion::SString, anarion::SString> anarion::FilePayload::suffix2ContentType({
    {SString("html"), SString("text/html")},
    {SString("css"), SString("text/css")},
    {SString("js"), SString("text/javascript")},

    // images
    {SString("apng"), SString("image/apng")},
    {SString("bmp"), SString("image/bmp")},
    {SString("gif"), SString("image/gif")},
    {SString("ico"), SString("image/x-icon")},
    {SString("cur"), SString("image/x-icon")},
    {SString("jpg"), SString("image/jpeg")},
    {SString("jpeg"), SString("image/jpeg")},
    {SString("jfif"), SString("image/jpeg")},
    {SString("pjpeg"), SString("image/jpeg")},
    {SString("pjp"), SString("image/jpeg")},
    {SString("png"), SString("image/png")},
    {SString("svg"), SString("image/svg+xml")},
    {SString("tiff"), SString("image/tiff")},
    {SString("tif"), SString("image/tiff")},
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
//    printf("%d %d %d\n", fileChannel->valid(), fileChannel->i_valid);
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

void anarion::FilePayload::unload_impl() {
    operator delete (fileCache, fileLength);
    fileCache = nullptr;
    fileLength = 0;
    lastLoadTime = {0, 0};
}

