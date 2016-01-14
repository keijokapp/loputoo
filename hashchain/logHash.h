#pragma once
#include <postgres.h>


#ifdef __cplusplus
extern "C" {
#endif

void logHash(const Relation, const HeapTuple); 

#ifdef __cplusplus
}
#endif

