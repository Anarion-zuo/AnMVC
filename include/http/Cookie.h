//
// Created by anarion on 5/1/20.
//

#ifndef MYMVC_COOKIE_H
#define MYMVC_COOKIE_H

#include <container/Map/HashMap.hpp>
#include <container/SString.h>

namespace anarion {
    class Cookie {
    protected:
        HashMap<SString, SString> _map;

    public:

        void put(SString key, SString val);
        HashMap<SString, SString>::iterator get(const SString &key);

    };
}

#endif //MYMVC_COOKIE_H
