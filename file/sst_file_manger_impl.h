//
// Created by yuganchao on 1/15/20.
//
#pragma once

#include "fstgraph/sst_file_manager.h"

namespace fstgraph {

    class SstFileManagerImpl : public SstFileManager {
    public:
        explicit SstFileManagerImpl(std::shared_ptr<FileSystem> fs);

        ~SstFileManagerImpl();

        Status OnAddFile(const std::string& file_path);

        Status OnDeleteFile(const std::string& file_path);

        Status OnMoveFile(const std::string& old_path, const std::string& new_path,
                uint64_t* file_size = nullptr);


    private:
        std::shared_ptr<FileSystem> fs_;
        uint64_t total_files_size_;
        uint64_t in_progress_files_size_;



    };



}
