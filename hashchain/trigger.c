#include <syslog.h>

#include <postgres.h>
#include <executor/spi.h>
#include <commands/trigger.h>
#include <utils/rel.h>
#include <fmgr.h>
PG_MODULE_MAGIC;

#ifndef SHIP_HASH_ONLY
#include "hashRecord.h"
#else
#include "shipHash.h"
#endif


#ifdef __cplusplus
#define DECLARE_TRIGGER(name) PG_FUNCTION_INFO_V1(name); extern "C" Datum name(PG_FUNCTION_ARGS)
#else
#define DECLARE_TRIGGER(name) PG_FUNCTION_INFO_V1(name); extern Datum name(PG_FUNCTION_ARGS)
#endif

void __attribute__((constructor)) init() {
	openlog("hashchainTrigger", LOG_NDELAY, LOG_LOCAL1);
	syslog(LOG_INFO, "hashchain module loaded");
}



DECLARE_TRIGGER(hashchainTrigger) {
	if(!CALLED_AS_TRIGGER(fcinfo)) elog(ERROR, "hashchainTrigger: not called by trigger manager");

	TriggerData* trigdata = (TriggerData*) fcinfo->context;

	if(!TRIGGER_FIRED_FOR_ROW(trigdata->tg_event) || !TRIGGER_FIRED_BY_INSERT(trigdata->tg_event)) {
		elog(ERROR, "hashchainTrigger: only for row-level INSERT events");
		return PointerGetDatum(TRIGGER_FIRED_BY_UPDATE(trigdata->tg_event) ? trigdata->tg_newtuple : trigdata->tg_trigtuple);
	}

#ifndef SHIP_HASH_ONLY
	if(!TRIGGER_FIRED_BEFORE(trigdata->tg_event)) {
		elog(ERROR, "hashchainTrigger: only for row-level before-INSERT events");
		return 0;
	}

	HeapTuple inputRow = trigdata->tg_trigtuple;
	Relation rel = trigdata->tg_relation;

	HeapTuple modifiedRow = hashRecord(rel, inputRow);
	return PointerGetDatum(modifiedRow);
#else
	if(!TRIGGER_FIRED_AFTER(trigdata->tg_event)) {
		elog(ERROR, "hashchainTrigger: only for row-level after-INSERT events");
		return 0;
	}

	HeapTuple inputRow = trigdata->tg_trigtuple;
	Relation rel = trigdata->tg_relation;

	int result = shipHash(rel, inputRow);
	if(result < 0) {
		syslog(LOG_ERR, "Failed to ship last hash");
	}
	
	return 0;
#endif
}
