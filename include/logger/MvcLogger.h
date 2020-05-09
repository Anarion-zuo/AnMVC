//
// Created by anarion on 5/9/20.
//

#ifndef MYMVC_MVCLOGGER_H
#define MYMVC_MVCLOGGER_H

#include <logger/Logger.h>
#include <io/channel/file/FileChannel.h>
#include <io/channel/terminal/TerminalPrintChannel.h>

namespace anarion {
    class MvcLogger : public Logger {
    protected:
        SString loggerFileDir;
    public:
        explicit MvcLogger(SString &&loggerFileDir) {
            // register looger file
            outputs.push_back(new FileChannel(FileChannel::open(forward<SString>(loggerFileDir))));
            // terminal
            outputs.push_back(new TerminalPrintChannel());
        }
    };

    class MvcLoggerInfo : public LoggerInfo {
    protected:
        Time time;
    public:
        MvcLoggerInfo() {
            time.setCurrent();
        }
    };
}

#endif //MYMVC_MVCLOGGER_H
