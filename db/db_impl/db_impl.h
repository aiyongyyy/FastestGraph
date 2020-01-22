//
// Created by yuganchao on 1/8/20.
//

#pragma once

#include "fstgraph/db.h"
#include "fstgraph/options.h"
#include "fstgraph/file_system.h"


namespace fstgraph {



    class DBImpl : public DB {
    public:
        DBImpl(const DBOptions& options, const std::string &dbname);

        virtual ~DBImpl();


        static Status Open(const DBOptions& db_options, const std::string& name,
                           DB** dbptr);
        const Status CreateArchivalDirectory();

    protected:
        const std::string dbname_;

        const DBOptions initial_db_options_;
        std::shared_ptr<FileSystem> fs_;

    };


    extern DBOptions SanitizeOptions(const std::string& db, const DBOptions& src);

}

