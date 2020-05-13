//
// Created by anarion on 5/13/20.
//

#ifndef MYMVC_REQUESTROUTER_H
#define MYMVC_REQUESTROUTER_H

#include <base/container/Map/HashMap.hpp>
#include <base/container/SString.h>
#include <base/parser/ListParser.h>

namespace anarion {
    class RequestRouter {
    protected:
        HashMap<SString, RequestRouter*> dirMap;
        HashMap<SString, RequestRouter*> cacheMap;

        static ListParser *parser;
        static LinkedList<SString> parseDirectory(const SString &dir);

    public:
        void addMapping(SString &&dir, RequestRouter *router);
        RequestRouter *getChild(SString &&dir);
    };
}

#endif //MYMVC_REQUESTROUTER_H
