//
// Created by yuganchao on 1/8/20.
//

#pragma once

#include "fstgraph/status.h"
#include "fstgraph/options.h"


namespace fstgraph {


    class DB {
    public:
        // Open the database with the specified "name".
        // Stores a pointer to a heap-allocated database in *dbptr and returns
        // OK on success.
        // Stores nullptr in *dbptr and returns a non-OK status onerror.
        // Caller should delete *dbptr when it is no longer needed.
        static Status Open(const DBOptions& options, const std::string& name,
                           DB** dbptr);


    };

}