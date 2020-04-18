//
// Created by anarion on 2020/4/1.
//

//#include "parser/MapParser.h"
//
//anarion::MapParser::MapParser(char equal, char separator) : equal(equal), separator(separator) {}
//
//anarion::HashMap<anarion::SString, anarion::SString> anarion::MapParser::parse(anarion::SString &text) {
//    HashMap<SString, SString> ret;
//    size_type sep_index = text.indexOfSince(separator, 0);
//    size_type equ_index = text.indexOfSince(equal, 0);
//    size_type left_index = 0;
//    while (true) {
//        // skip redundant characters
//        left_index = text.indexSkip(" ", left_index);
//        equ_index = text.indexSkip(" ", equ_index);
//        // extract substrings
//        SString key;
//        key.assign(text.begin_iterator() + left_index, text.begin_iterator() + equ_index);
//        sep_index = text.indexSkip(" ", sep_index + 1);
//        SString val;
//        val.assign(text.begin_iterator() + equ_index + 1, text.begin_iterator() + sep_index);
//        // inject data
//        ret.put(move(key), move(val));
//        // check ending
//        if (sep_index == text.size()) {
//            break;
//        }
//        // move to next part
//        left_index = sep_index + 1;
//        equ_index = text.indexOfSince(equal, left_index);
//        sep_index = text.indexOfSince(separator, left_index);
//    }
//    return move(ret);
//}
