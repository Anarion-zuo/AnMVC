//
// Created by anarion on 5/9/20.
//

#include "logger/MvcLogger.h"


void anarion::MvcLoggerInfo::append(const SString &message) {
    messageBuffer.append_string(message);
}

void anarion::MvcLoggerInfo::append(const char *str, anarion::size_type length) {
    messageBuffer.append_arr(const_cast<char*>(str), length);
}

void anarion::MvcLoggerInfo::appendChar(char c, anarion::size_type n) {
    char *cs = static_cast<char *>(operator new(n));
    memset(cs, c, n);
    messageBuffer.append_arr(cs, n);
    operator delete (cs, n);
}

void anarion::MvcLoggerInfo::toChannel(anarion::InChannel &inChannel) {
    memberBuffer.rewind();
    messageBuffer.rewind();
    inChannel.in(memberBuffer);
    inChannel.in(messageBuffer);
}

void anarion::MvcLoggerInfo::serialize() {
    // data member serialization
    Date date(time);
    SString dateString = date.print();
    memberBuffer.append_c('[');
    memberBuffer.append_string(dateString);
    memberBuffer.append_c(']');
    memberBuffer.append_c(' ');

    // place together
}
