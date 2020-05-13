//
// Created by anarion on 5/13/20.
//

#include "http/RequestRouter.h"

anarion::ListParser *anarion::RequestRouter::parser = new ListParser('/');

anarion::RequestRouter *anarion::RequestRouter::getChild(anarion::SString &&dir) {
    // search cache first
    auto cacheIt = cacheMap.find(dir);
    if (cacheIt != cacheMap.end_iterator()) {
        return cacheIt->get_val();
    }
}

void anarion::RequestRouter::addMapping(anarion::SString &&dir, anarion::RequestRouter *router) {

}

anarion::LinkedList<anarion::SString> anarion::RequestRouter::parseDirectory(const anarion::SString &dir) {
    return parser->parse(dir);
}
