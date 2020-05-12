//
// Created by anarion on 4/15/20.
//

#include "resource/StaticResources.h"
#include <io/channel/file/FileChannel.h>

anarion::SString anarion::StaticResources::htmlString("html"), anarion::StaticResources::cssString("css"), anarion::StaticResources::jsString("js"), anarion::StaticResources::imgString("img");

anarion::HashSet<anarion::SString> anarion::StaticResources::imageFormatSet {
        SString("apng"), SString("bmp"), SString("gif"), SString("ico"), SString("cur"), SString("jpg"), SString("jpeg"), SString("jfif"), SString("pjpeg"), SString("pjp"), SString("png"), SString("svg"), SString("tiff"), SString("tif"), SString("webp")
};

anarion::StaticResources::StaticResources(anarion::SString &&dir) : directory(forward<SString>(dir)) {
    directory.open();

    htmlDirectory = dynamic_cast<Directory *>(directory.getChild(htmlString));
    if (htmlDirectory) {
        name2dir.put(htmlString, htmlDirectory);
    }
    cssDirectory = dynamic_cast<Directory *>(directory.getChild(cssString));
    if (cssDirectory) {
        name2dir.put(cssString, cssDirectory);
    }
    jsDirectory = dynamic_cast<Directory *>(directory.getChild(jsString));
    if (jsDirectory) {
        name2dir.put(jsString, jsDirectory);
    }
    imgDirectory = dynamic_cast<Directory *>(directory.getChild(imgString));
    if (imgDirectory) {
        name2dir.put(imgString, imgDirectory);
    }
}

anarion::FileChannel * anarion::StaticResources::getFile(const anarion::SString &relDir) {
    SString suffix = relDir.suffix('.');

    // images
    auto imageIt = imageFormatSet.find(suffix);
    if (imageIt != imageFormatSet.end_iterator()) {
        FileEntry *entry;
        if (imgDirectory) {
            entry = imgDirectory->findByDir(relDir);
        } else {
            entry = directory.findByDir(relDir);
        }
        if (entry) {
            if (entry->isFile()) {
                return dynamic_cast<FileChannel *>(entry);
            }
        }
    }

    // others
    Directory *childDirectory = getChildDirectoryByName(suffix);
    if (childDirectory) {
        FileEntry *entry = childDirectory->findByDir(relDir);
        if (entry) {
            if (entry->isFile()) {
                return dynamic_cast<FileChannel *>(entry);
            }
        }
    }
    FileEntry *entry = directory.findByDir(relDir);
    if (entry == nullptr) {
        return nullptr;
    }
    if (!entry->isFile()) {
        return nullptr;
    }
    return dynamic_cast<FileChannel *>(entry);
}

anarion::Directory *anarion::StaticResources::getChildDirectoryByName(const anarion::SString &name) {
    auto it = name2dir.find(name);
    if (it == name2dir.end_iterator()) {
        Directory *d = dynamic_cast<Directory *>(directory.getChild(name));
        if (d == nullptr) {
            return nullptr;
        }
        if (d->isFile()) {
            return nullptr;
        }
        return d;
    }
    Directory *d = it->get_val();
    if (d->isFile()) {
        return nullptr;
    }
    return d;
}

anarion::Payload *anarion::StaticResources::getPayload(const anarion::SString &relDir) {
    auto it = dir2payload.find(relDir);
    if (it != dir2payload.end_iterator()) {
        Payload *ret = it->get_val();
        loadPayload(ret);
        return ret;
    }
    // update payload table
    FileEntry *entry = getFile(relDir);
    if (entry == nullptr) {
        return nullptr;
    }
    FilePayload *payload = new FilePayload(dynamic_cast<FileChannel*>(entry));
    payload->setContentTypeBySuffix(entry->getSuffix());
    loadPayload(payload);
    dir2payload.put(relDir, payload);
    return payload;
}

void anarion::StaticResources::loadPayload(anarion::Payload *payload) {
    manager->load(payload);
}
