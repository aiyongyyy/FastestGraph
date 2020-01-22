//
// Created by yuganchao on 1/14/20.
//

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>


#if defined(OS_LINUX)
#include <linux/fs.h>
#endif
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#if defined(OS_LINUX) || defined(OS_SOLARIS) || defined(OS_ANDROID)
#include <sys/statfs.h>
#include <sys/syscall.h>
#include <sys/sysmacros.h>
#endif
#include <sys/statvfs.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <algorithm>
// Get nano time includes
#if defined(OS_LINUX) || defined(OS_FREEBSD)
#elif defined(__MACH__)
#include <Availability.h>
#include <mach/clock.h>
#include <mach/mach.h>
#else
#include <chrono>
#endif
#include <deque>
#include <set>
#include <vector>

#include "env/fs_posix.h"
#include "fstgraph/status.h"
#include "fstgraph/file_system.h"
#include "fstgraph/options.h"

#if !defined(TMPFS_MAGIC)
#define TMPFS_MAGIC 0x01021994
#endif
#if !defined(XFS_SUPER_MAGIC)
#define XFS_SUPER_MAGIC 0x58465342
#endif
#if !defined(EXT4_SUPER_MAGIC)
#define EXT4_SUPER_MAGIC 0xEF53
#endif


namespace fstgraph {






    class PosixFileSystem : FileSystem {


        Status CreateDir(const std::string &name) override {
            Status result;
            if (mkdir(name.c_str(), 0755) != 0) {

                result = Status::Error("While mkdir: " + name, strerror(errno));

            }
            return result;
        }

        Status GetAbsolutePath(const std::string& db_path, std::string* output_path) override {
            if (!db_path.empty() && db_path[0] == '/') {
                *output_path = db_path;
                return Status::OK();
            }

            char the_path[256];
            char* ret = getcwd(the_path, 256);
            if (ret == nullptr) {
                return Status::Error(strerror(errno));
            }

            *output_path = ret;
            return Status::OK();
        }

    };




    std::shared_ptr<FileSystem> FileSystem::Default() {
        static PosixFileSystem default_fs;
        static std::shared_ptr<PosixFileSystem> default_fs_ptr(
                &default_fs, [](PosixFileSystem*) {});
        return default_fs_ptr;
    }



}