//
// Created by anarion on 5/13/20.
//

#ifndef MYMVC_WEBCONFIGRESOURCES_H
#define MYMVC_WEBCONFIGRESOURCES_H

#include <base/io/channel/file/Directory.h>

namespace anarion {
    class WebConfigResources {
    protected:
        Directory directory;

        static SString mappingsFileName;

    public:
        explicit WebConfigResources(SString &&dir) : directory(forward<SString>(dir)) {}

        void readMapping();
    };
}

#endif //MYMVC_WEBCONFIGRESOURCES_H
