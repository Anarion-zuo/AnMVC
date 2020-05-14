//
// Created by anarion on 5/13/20.
//

#include "http/RequestRouter.h"

anarion::ListParser *anarion::RequestRouter::parser = new ListParser('/');

anarion::RequestRouter *anarion::RequestRouter::getChild(const SString &dir) const {
    auto it = dirMap.find(dir);
    if (it == dirMap.end_iterator()) {
        return nullptr;
    }
    RequestRouter * child = it->get_val();
    if (child == nullptr) {
        return nullptr;
    }
    return child;
}

void anarion::RequestRouter::addMapping(anarion::SString &&dir, anarion::RequestRouter *router) {
    auto it = dirMap.find(dir);
    if (it != dirMap.end_iterator()) {
        throw MappingAlreadyExists();
    }
    dirTrie.addWord(dir);
    dirMap.put(anarion::forward<SString>(dir), router);
}

anarion::LinkedList<anarion::SString> anarion::RequestRouter::parseDirectory(const anarion::SString &dir) {
    return parser->parse(dir);
}

anarion::Controller *anarion::RequestRouter::query(const anarion::SString &dir) {
    RequestRouter *router = this;
    size_type leftIndex = 0;
    while (leftIndex != dir.length()) {
        size_type prefixLength = dirTrie.longestPrefix(dir, leftIndex);
        router = router->getChild(SString(dir.getArr() + leftIndex, prefixLength));
        if (router == nullptr) {
            return nullptr;
        }
        leftIndex += prefixLength;
    }
    return router->controller;
}

