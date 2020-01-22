//
// Created by yuganchao on 1/14/20.
//

#pragma once

#include <memory>
#include "fstgraph/status.h"

namespace fstgraph {


    class FileSystem {
    public:
        FileSystem();

        virtual Status CreateDir(const std::string& dirname) = 0;

        virtual Status GetAbsolutePath(const std::string& db_path, std::string* output_path) = 0;

        static std::shared_ptr<FileSystem> Default();


    };


}
