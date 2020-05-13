//
// Created by anarion on 5/13/20.
//

#include <base/io/channel/file/FileChannel.h>
#include <base/parser/json/JsonObject.h>
#include <base/parser/json/JsonMap.h>
#include "resource/WebConfigResources.h"

anarion::SString anarion::WebConfigResources::mappingsFileName("mappings.json");

#define cleanup_read_mapping \
delete json;\
operator delete (mappingJson, fileSize);\
file->close();\
return;

void anarion::WebConfigResources::readMapping() {
    FileEntry *entry = directory.getChild(mappingsFileName);
    if (entry == nullptr) {
        return ;
    }
    FileChannel *file = dynamic_cast<FileChannel*>(entry);
    size_type fileSize = file->size();

    char *mappingJson = static_cast<char *>(operator new(fileSize));
    JsonMap *json = JsonObject::parse(mappingJson, fileSize);
    auto it = json->find(SString("mappings"));
    if (it == json->end_iterator()) {
        cleanup_read_mapping
    }

    cleanup_read_mapping
}
