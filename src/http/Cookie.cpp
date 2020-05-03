//
// Created by anarion on 5/1/20.
//

#include "http/Cookie.h"

void anarion::Cookie::put(anarion::SString key, anarion::SString val) {
    _map.put(move(key), move(val));
}

anarion::HashMap<anarion::SString, anarion::SString>::iterator anarion::Cookie::get(const anarion::SString &key) {
    return _map.find(key);
}
