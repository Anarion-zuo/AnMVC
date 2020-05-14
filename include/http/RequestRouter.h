//
// Created by anarion on 5/13/20.
//

#ifndef MYMVC_REQUESTROUTER_H
#define MYMVC_REQUESTROUTER_H

#include <base/container/Map/HashMap.hpp>
#include <base/container/SString.h>
#include <base/parser/ListParser.h>
#include <controller/Controller.h>
#include <base/container/Trie.h>
#include "HttpComponent.h"

namespace anarion {
    class RequestRouter : public HttpComponent {
    protected:
        HashMap<SString, RequestRouter*> dirMap;
        Trie dirTrie;
        Controller *controller = nullptr;

        static ListParser *parser;
        static LinkedList<SString> parseDirectory(const SString &dir);

    public:

        explicit RequestRouter(HttpContext *context) : HttpComponent(context) {}

        void addMapping(SString &&dir, RequestRouter *router);
        RequestRouter *getChild(const SString &dir) const ;  // return nullptr if not found
        Controller *query(const SString &dir);
        constexpr void setController(Controller *controller) { this->controller = controller; }
    };

    struct RequestRouterException : std::exception {};
    struct MappingAlreadyExists : public RequestRouterException {
        const char *what() const noexcept override {
            return "Request mapping already exists";
        }
    };
    struct MappingDoesNotExist : public RequestRouterException {
        const char *what() const noexcept override {
            return "Request mapping does not exist";
        }
    };
}

#endif //MYMVC_REQUESTROUTER_H
