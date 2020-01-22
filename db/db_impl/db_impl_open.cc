//
// Created by yuganchao on 1/13/20.
//

#include "db/db_impl/db_impl.h"
#include "fstgraph/status.h"

namespace fstgraph {

DBOptions SanitizeOptions(const std::string& dbname, const DBOptions& src) {

    DBOptions result(src);

    if (result.file_system == nullptr) {
        result.file_system = FileSystem::Default();
    }

    if (result.db_paths.size() == 0) {
        result.db_paths.emplace_back(dbname, std::numeric_limits<uint64_t>::max());
    }

    if (result.wal_dir.empty()) {
        // Use dbname as default
        result.wal_dir = dbname;
    }

    return result;
}

Status DBImpl::Open(const fstgraph::DBOptions &db_options, const std::string &name, fstgraph::DB **dbptr){


    *dbptr = nullptr;

    size_t max_write_buffer_size = 0;


    DBImpl* impl = new DBImpl(db_options, name);

    Status s = impl->fs_->CreateDir(impl->initial_db_options_.wal_dir);
    if (s.ok()) {
        std::vector<std::string> paths;
        for (auto& db_path : impl->initial_db_options_.db_paths) {
            paths.emplace_back(db_path.path);
        }

        for (auto& path : paths) {
            s = impl->fs_->CreateDir(path);
            if (!s.ok()) {
                break;
            }
        }
    }

    if (!s.ok()) {
        delete impl;
        return s;
    }

    s = impl->CreateArchivalDirectory();
    if (!s.ok()) {
        delete impl;
        return s;
    }

    *dbptr = impl;

    auto sfm = static_cast<SstFileManagerImpl*>(
            impl->immutable_db_options_.sst_file_manager.get());
    if (s.ok() && sfm) {
        // Notify SstFileManager about all sst files that already exist in
        // db_paths[0] and cf_paths[0] when the DB is opened.
        std::vector<std::string> paths;
        paths.emplace_back(impl->immutable_db_options_.db_paths[0].path);
        for (auto& cf : column_families) {
            if (!cf.options.cf_paths.empty()) {
                paths.emplace_back(cf.options.cf_paths[0].path);
            }
        }
        // Remove duplicate paths.
        std::sort(paths.begin(), paths.end());
        paths.erase(std::unique(paths.begin(), paths.end()), paths.end());
        for (auto& path : paths) {
            std::vector<std::string> existing_files;
            impl->immutable_db_options_.env->GetChildren(path, &existing_files);
            for (auto& file_name : existing_files) {
                uint64_t file_number;
                FileType file_type;
                std::string file_path = path + "/" + file_name;
                if (ParseFileName(file_name, &file_number, &file_type) &&
                    file_type == kTableFile) {
                    sfm->OnAddFile(file_path);
                }
            }
        }

        // Reserve some disk buffer space. This is a heuristic - when we run out
        // of disk space, this ensures that there is atleast write_buffer_size
        // amount of free space before we resume DB writes. In low disk space
        // conditions, we want to avoid a lot of small L0 files due to frequent
        // WAL write failures and resultant forced flushes
        sfm->ReserveDiskBuffer(max_write_buffer_size,
                               impl->immutable_db_options_.db_paths[0].path);
    }!ROCKSDB_LITE

    if (s.ok()) {
        ROCKS_LOG_HEADER(impl->immutable_db_options_.info_log, "DB pointer %p",
                         impl);
        LogFlush(impl->immutable_db_options_.info_log);
        assert(impl->TEST_WALBufferIsEmpty());
        // If the assert above fails then we need to FlushWAL before returning
        // control back to the user.
        if (!persist_options_status.ok()) {
            s = Status::IOError(
                    "DB::Open() failed --- Unable to persist Options file",
                    persist_options_status.ToString());
        }
    }
    if (s.ok()) {
        impl->StartTimedTasks();
    }
    if (!s.ok()) {
        for (auto* h : *handles) {
            delete h;
        }
        handles->clear();
        delete impl;
        *dbptr = nullptr;
    }
    return s;
}
}