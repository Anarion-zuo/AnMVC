//
// Created by anarion on 5/3/20.
//

#include "service/BackgroundService.h"
#include "../../../AnBase/include/time/Date.h"

void anarion::BackgroundService::task() {
    updateTimeDate();
}

void anarion::BackgroundService::updateTimeDate() {
    timeLock.lock();
    currentTime.setCurrent();
    currentDate.setTime(currentTime);
    timeLock.unlock();
}

anarion::BackgroundService::BackgroundService(Time &&time) : BackgroundTask(anarion::forward<Time>(time)) {}

anarion::Time anarion::BackgroundService::getTime() {
    timeLock.lock();
    Time ret = currentTime;
    timeLock.unlock();
    return ret;
}

anarion::Date anarion::BackgroundService::getDate() {
    timeLock.lock();
    Date ret = currentDate;
    timeLock.unlock();
    return ret;
}
