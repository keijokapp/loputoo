#pragma once
#include <postgres.h>


#ifdef __cplusplus
extern "C" {
#endif

HeapTuple hashRecord(const Relation, const HeapTuple); 

#ifdef __cplusplus
}
#endif

