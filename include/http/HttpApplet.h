//
// Created by anarion on 2020/3/31.
//

#ifndef MYMVC_HTTPAPPLET_H
#define MYMVC_HTTPAPPLET_H

#include <concurrent/container/Queue/BlockQueue.hpp>
#include <resource/StaticResources.h>
#include <resource/TextPayload.h>
#include "Request.h"
#include "Response.h"

namespace anarion {
    class HttpApplet {
    protected:

        template <class ItemType>
        class ItemPool {
        private:
            BlockQueue<ItemType*> appQueue;
        public:
            ItemPool() {
                appQueue.push(new ItemType);
            }

            ItemType *fetch() {
                if (appQueue.empty()) {
                    return new ItemType;
                }
                return appQueue.pop();
            }

            void returnItem(ItemType *app) {
                appQueue.push(app);
            }
        };

        Request *request = nullptr;
        Response *response = nullptr;

        /*
         * request handling interface
         */
        virtual void onGet() {};
        virtual void onPost() {};
        virtual void onDelete() {};
        virtual void onPut() {};
        virtual void onHead() {};
        virtual void onConnect() {};
        virtual void onOptions() {};
        virtual void onTrace() {};
        virtual void onPatch() {};
        virtual void onUnknown() {};

        static StaticResources staticResources;

        // error pages
        static TextPayload page400Text;
        void set400Page(int status);

    public:

        HttpApplet() {}
        virtual ~HttpApplet() {}

        constexpr void setRequest(Request &in_request) { this->request = &in_request; }

        virtual HttpApplet *getInstance() = 0;
        virtual void release() { delete this; }   // override please
        void process();
        constexpr Response *getResponse() const { return response; }
    };
}

#endif //MYMVC_HTTPAPPLET_H
