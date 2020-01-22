//
// Created by yuganchao on 1/14/20.
//

#pragma once

#include "assert.h"
#include <stddef.h>
#include <string>
#include "string.h"

#ifdef __cpp_lib_string_view
#include <string_view>
#endif

namespace fstgraph {

    class Slice {
    public:
        // Create an empty slice.
        Slice() : data_(""), size_(0) {}

        Slice(const char* d, size_t n) : data_(d), size_(n) {}

        Slice(const std::string& s) : data_(s.data()), size_(s.size()) {}

        Slice(const char* s) : data_(s) { size_ = (s == nullptr) ? 0 : strlen(s); }

#ifdef __cpp_lib_string_view
        // Create a slice that refers to the same contents as "sv"
  /* implicit */
  Slice(std::string_view sv) : data_(sv.data()), size_(sv.size()) {}
#endif

        const char* data() const { return data_; }

        // Return the length (in bytes) of the referenced data
        size_t size() const { return size_; }


        char operator[](size_t n) const {
            assert(n < size());
            return data_[n];
        }

        // Change this slice to refer to an empty array
        void clear() {
            data_ = "";
            size_ = 0;
        }

        // Drop the first "n" bytes from this slice.
        void remove_prefix(size_t n) {
            assert(n <= size());
            data_ += n;
            size_ -= n;
        }

        void remove_suffix(size_t n) {
            assert(n <= size());
            size_ -= n;
        }


        const char* data_;
        size_t size_;

    };

}