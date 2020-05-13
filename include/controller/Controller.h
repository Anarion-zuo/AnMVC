//
// Created by anarion on 5/12/20.
//

#ifndef MYMVC_CONTROLLER_H
#define MYMVC_CONTROLLER_H

#include <base/container/SString.h>
#include <container/Map/HashMap.hpp>

namespace anarion {
    class Controller {
    protected:
        static HashMap<SString, Controller*> nameMap;
    };
}

#endif //MYMVC_CONTROLLER_H
