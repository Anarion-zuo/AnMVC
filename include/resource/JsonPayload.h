//
// Created by anarion on 5/1/20.
//

#ifndef MYMVC_JSONPAYLOAD_H
#define MYMVC_JSONPAYLOAD_H

#include <parser/json/JsonObject.h>
#include "Payload.h"

namespace anarion {
    class JsonPayload : virtual public Payload {
    protected:
        JsonObject *jsonObj = nullptr;
        SString serialized;

        static SString contentType, contentEncoding;
    public:
        void send(SocketChannel &channel) override;

        size_type length() override;
        void load() override;
        void unload() override;
        bool isLoaded() override;

        const SString &getContentType() override;\
        const SString &getContentEncoding() override;

        JsonPayload() = default;
        explicit JsonPayload(JsonObject *object) : jsonObj(object) {}
        JsonPayload(JsonPayload &&rhs) noexcept : jsonObj(rhs.jsonObj), serialized(move(rhs.serialized)) { rhs.jsonObj = nullptr; }

        JsonObject *getJsonObj() const {
            return jsonObj;
        }

        void setJsonObj(JsonObject *jsonObj) {
            JsonPayload::jsonObj = jsonObj;
        }
    };
}

#endif //MYMVC_JSONPAYLOAD_H
