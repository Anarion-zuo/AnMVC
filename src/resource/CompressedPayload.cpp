//
// Created by anarion on 2020/4/2.
//

#include "resource/CompressedPayload.h"
#include <zlib.h>
anarion::SString anarion::CompressedPayload::defaultContentEncoding("deflate");

void anarion::CompressedPayload::load() {
    FilePayload::load();
    size_type outbound = compressBound(fileLength), outlen = outbound;
    int ret;
    char *outbuf = static_cast<char *>(operator new(outbound));
    ret = ::compress(reinterpret_cast<Bytef *>(outbuf), &outlen, reinterpret_cast<const Bytef *>(fileCache), fileLength);
    if (ret != Z_OK) {
        // TODO compress failed
    }
    if (compressedCache) {  // release old data
        operator delete (compressedCache, compressedSpaceLength);
    }
    // switch to new data
    compressedSpaceLength = outbound;
    compressedLength = outlen;
    compressedCache = outbuf;

    operator delete (fileCache, fileLength);  // release uncompressed data
}

anarion::size_type anarion::CompressedPayload::length() {
    return compressedLength;
}

const anarion::SString &anarion::CompressedPayload::getContentEncoding() {
    return defaultContentEncoding;
}

bool anarion::CompressedPayload::isLoaded() {
    return compressedCache;
}

anarion::CompressedPayload::CompressedPayload(FileChannel *fileChannel) : FilePayload(fileChannel) {}

void anarion::CompressedPayload::send(anarion::SocketChannel &channel) {
    if (needUpdate()) {
        load();
    }
    channel.in(compressedCache, compressedLength);
}

void anarion::CompressedPayload::unload_impl() {
    FilePayload::unload_impl();  // unload parent fields
    // unload this fields
    operator delete (compressedCache, compressedSpaceLength);
    compressedCache = nullptr;
    compressedSpaceLength = 0;
    compressedLength = 0;
}


