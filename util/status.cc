//
// Created by yuganchao on 1/14/20.
//

#include "fstgraph/status.h"
#include <stdio.h>
#ifdef OS_WIN
#include <string.h>
#endif
#include <cstring>

namespace fstgraph {

    Status::Status(Code _code, SubCode _subcode, const Slice &msg,
                   const Slice &msg2)
            : code_(_code), subcode_(_subcode), sev_(kNoError) {
        assert(code_ != kOk);
        assert(subcode_ != kMaxSubCode);
        const size_t len1 = msg.size();
        const size_t len2 = msg2.size();
        const size_t size = len1 + (len2 ? (2 + len2) : 0);
        char *const result = new char[size + 1];  // +1 for null terminator
        memcpy(result, msg.data(), len1);
        if (len2) {
            result[len1] = ':';
            result[len1 + 1] = ' ';
            memcpy(result + len1 + 2, msg2.data(), len2);
        }
        result[size] = '\0';  // null terminator for C style string
        state_ = result;
    }

}