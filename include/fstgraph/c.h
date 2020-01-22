//
// Created by yuganchao on 1/8/20.
//

#pragma once

#ifdef _WIN32
#ifdef FSTGRAPH_DLL
#ifdef FSTGRAPH_LIBRARY_EXPORTS
#define FSTGRAPH_LIBRARY_API __declspec(dllexport)
#else
#define FSTGRAPH_LIBRARY_API __declspec(dllimport)
#endif
#else
#define FSTGRAPH_LIBRARY_API
#endif
#else
#define FSTGRAPH_LIBRARY_API
#endif

#ifdef __cplusplus
extern "C" {
#endif


typedef struct fstgraph_t                 fstgraph_t;
typedef struct fstgraph_cache_t           fstgraph_cache_t;



extern FSTGRAPH_LIBRARY_API fstgraph_t* fstgraph_open(
        const char* name, char** errptr);



#ifdef __cplusplus
}  /* end extern "C" */
#endif

