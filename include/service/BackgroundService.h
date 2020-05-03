//
// Created by anarion on 5/3/20.
//

#ifndef MYMVC_BACKGROUNDSERVICE_H
#define MYMVC_BACKGROUNDSERVICE_H

#include <concurrent/base/BackgroundTask.h>
#include <time/Date.h>
#include <concurrent/base/Mutex.h>
#include <time/Time.h>
#include <time/Date.h>

namespace anarion {
    class BackgroundService : public virtual BackgroundTask {
    protected:
        Time currentTime;
        Date currentDate;
        Mutex timeLock;     // for time and date
        void updateTimeDate();
    public:

        explicit BackgroundService(Time &&time);

        void task() override;

        Time getTime();
        Date getDate();
    };
}

#endif //MYMVC_BACKGROUNDSERVICE_H
