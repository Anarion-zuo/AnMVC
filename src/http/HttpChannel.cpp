//
// Created by anarion on 2020/4/2.
//

#include "http/HttpChannel.h"

//void anarion::HttpAliveChannelQueue::CleaningThread::run() {
//    while (true) {
//        // scan the whole queue
//        _this->lock.lock();
//        for (auto it = _this->aliveList.begin_iterator(); it != _this->aliveList.end_iterator(); ++it) {
//            if ((*it)->isTimeOut()) {
//            }
//        }
//        _this->lock.unlock();
//        // sleep
//        _this->cond.waitTime({60 * 2, 0});
//    }
//}

void anarion::HttpChannel::loadRequest(Request *request) {
    timeOut = request->getKeepAliveTimeOut();
}

void anarion::HttpAliveChannelQueue::addChannel(HttpChannel *httpChannel) {
    aliveList.push_back(httpChannel);
}


