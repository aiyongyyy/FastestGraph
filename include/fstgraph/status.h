//
// Created by yuganchao on 1/8/20.
//

#pragma once
#include <string>
#include "fstgraph/slice.h"

namespace fstgraph {

class Status {
    public:
        // Create a success status.
        Status() : code_(kOk), subcode_(kNone), sev_(kNoError), state_(nullptr) {}
        ~Status() { delete[] state_; }



        // Copy the specified status.
        Status(const Status& s);
        Status& operator=(const Status& s);
        Status(Status&& s);
        Status& operator=(Status&& s);
        bool operator==(const Status& rhs) const;
        bool operator!=(const Status& rhs) const;

        enum Code : unsigned char {
            kOk = 0,
            kCodeErr = 1,
            kMaxCode
        };

        Code code() const { return code_; }

        enum SubCode : unsigned char {
            kNone = 0,
            kSubCodeErr = 1,
            kMaxSubCode
        };

        SubCode subcode() const { return subcode_; }

        enum Severity : unsigned char {
            kNoError = 0,
            kSevErr = 1,
            kMaxSeverity
        };

        Status(Code _code, SubCode _subcode, const Slice& msg, const Slice& msg2);
        Status(Code _code, const Slice& msg, const Slice& msg2)
                : Status(_code, kNone, msg, msg2) {}

        Status(const Status& s, Severity sev);
        Severity severity() const { return sev_; }

        // Returns a C style string indicating the message of the Status
        const char* getState() const { return state_; }

        // Return a success status.
        static Status OK() { return Status(); }

        // Return a success status.
        static Status Error() { return Status(kCodeErr); }

        static Status Error(const Slice& msg, const Slice& msg2 = Slice()) {
            return Status(kCodeErr, msg, msg2);
        }

        // Returns true iff the status indicates success.
        bool ok() const { return code() == kOk; }

        // Return a string representation of this status suitable for printing.
        // Returns the string "OK" for success.
        std::string ToString() const;

    protected:
        // A nullptr state_ (which is always the case for OK) means the message
        // is empty.
        // of the following form:
        //    state_[0..3] == length of message
        //    state_[4..]  == message
        Code code_;
        SubCode subcode_;
        Severity sev_;
        const char* state_;

        explicit Status(Code _code, SubCode _subcode = kNone)
                : code_(_code), subcode_(_subcode), sev_(kNoError), state_(nullptr) {}

        Status(Code _code, SubCode _subcode, const Slice& msg, const Slice& msg2);
        Status(Code _code, const Slice& msg, const Slice& msg2)
                : Status(_code, kNone, msg, msg2) {}

        static const char* CopyState(const char* s);
    };

    inline Status::Status(const Status& s)
            : code_(s.code_), subcode_(s.subcode_), sev_(s.sev_) {
        state_ = (s.state_ == nullptr) ? nullptr : CopyState(s.state_);
    }
    inline Status::Status(const Status& s, Severity sev)
            : code_(s.code_), subcode_(s.subcode_), sev_(sev) {
        state_ = (s.state_ == nullptr) ? nullptr : CopyState(s.state_);
    }
    inline Status& Status::operator=(const Status& s) {
        // The following condition catches both aliasing (when this == &s),
        // and the common case where both s and *this are ok.
        if (this != &s) {
            code_ = s.code_;
            subcode_ = s.subcode_;
            sev_ = s.sev_;
            delete[] state_;
            state_ = (s.state_ == nullptr) ? nullptr : CopyState(s.state_);
        }
        return *this;
    }

    inline Status::Status(Status&& s)
            : Status() {
        *this = std::move(s);
    }

    inline Status& Status::operator=(Status&& s)
#if !(defined _MSC_VER) || ((defined _MSC_VER) && (_MSC_VER >= 1900))
    noexcept
#endif
    {
        if (this != &s) {
            code_ = std::move(s.code_);
            s.code_ = kOk;
            subcode_ = std::move(s.subcode_);
            s.subcode_ = kNone;
            sev_ = std::move(s.sev_);
            s.sev_ = kNoError;
            delete[] state_;
            state_ = nullptr;
            std::swap(state_, s.state_);
        }
        return *this;
    }

    inline bool Status::operator==(const Status& rhs) const {
        return (code_ == rhs.code_);
    }

    inline bool Status::operator!=(const Status& rhs) const {
        return !(*this == rhs);
    }
}

