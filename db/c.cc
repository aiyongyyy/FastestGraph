//
// Created by yuganchao on 1/8/20.
//

#include "fstgraph/c.h"
#include "fstgraph/db.h"
#include "fstgraph/status.h"
#include <assert.h>
#include <string.h>

using fstgraph::DB;
using fstgraph::Status;


extern "C" {

struct fstgraph_t                 { DB*               rep; };


fstgraph_t* fstgraph_open(
        const char* name,
        char** errptr) {
    DB* db;
    if (SaveError(errptr, DB::Open(std::string(name), &db))) {
        return nullptr;
    }
    fstgraph_t* result = new fstgraph_t;
    result->rep = db;
    return result;
}


static bool SaveError(char** errptr, const Status& s) {
    assert(errptr != nullptr);
    if (s.ok()) {
        return false;
    } else if (*errptr == nullptr) {
        *errptr = strdup(s.ToString().c_str());
    } else {
        // TODO(sanjay): Merge with existing error?
        // This is a bug if *errptr is not created by malloc()
        free(*errptr);
        *errptr = strdup(s.ToString().c_str());
    }
    return true;
}

}

