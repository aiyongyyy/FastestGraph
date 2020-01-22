//
// Created by yuganchao on 1/15/20.
//
#pragma once

#include <memory>
#include "fstgraph/file_system.h"

namespace fstgraph {

    class SstFileManager {
    public:
        virtual ~SstFileManager() {}

        virtual uint64_t GetTotalSize() = 0;






    };

    extern SstFileManager* NewSstFileManager(
            std::shared_ptr<FileSystem> fs);



}