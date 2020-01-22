//
// Created by yuganchao on 1/8/20.
//

#include "db/db_impl/db_impl.h"
#include "fstgraph/options.h"


namespace fstgraph {


    DBImpl::DBImpl(const DBOptions &options, const std::string &dbname)
            : dbname_(dbname),
              initial_db_options_(SanitizeOptions(dbname, options)),
              fs_(initial_db_options_.file_system) {

    }

    const Status DBImpl::CreateArchivalDirectory() {

        std::string archivalPath = ArchivalDirectory(initial_db_options_.wal_dir);
        return fs_->CreateDir(archivalPath);
    }






}