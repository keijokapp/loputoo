#pragma once
#include <postgres.h>


#ifdef __cplusplus
extern "C" {
#endif

HeapTuple hashchain(const Relation, const HeapTuple); 

#ifdef __cplusplus
}
#endif

