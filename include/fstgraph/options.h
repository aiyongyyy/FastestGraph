//
// Created by yuganchao on 1/14/20.
//

#pragma once

#include <memory>
#include <vector>
#include "fstgraph/file_system.h"

namespace fstgraph {

    struct DbPath {
        std::string path;
        uint64_t target_size;  // Target size of total files under the path, in byte.

        DbPath() : target_size(0) {}

        DbPath(const std::string &p, uint64_t t) : path(p), target_size(t) {}
    };

    struct DBOptions {

        std::shared_ptr<FileSystem> file_system = nullptr;

        std::vector<DbPath> db_paths;

        std::string wal_dir = "";




    };







}
