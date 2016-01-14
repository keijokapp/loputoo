#include <string.h>
#include <syslog.h>
#include <openssl/sha.h>

#include <postgres.h>
#include <executor/spi.h>
#include <utils/rel.h>
#include <utils/builtins.h>

#include "logHash.h"

static const char hashchainFieldName[] = "hashchain";

/**
 * log hashchain value to system log (syslog)
 * @param rel relation (table)
 * @param row row to be inserted
 */
void logHash(const Relation rel, const HeapTuple row) {
	const char* tableName = SPI_getrelname(rel); // table name
	
	int hashchainFieldNumber = SPI_fnumber(rel->rd_att, hashchainFieldName); // hashchain field index
	int idFieldNumber = SPI_fnumber(rel->rd_att, "id"); // id field index

	if(hashchainFieldNumber <= 0) {
		elog(NOTICE, "hashchain triggered on table without hashchain field (%s)", tableName);
		return; // not hashchain'ed, no-op
	}

	/* send hash & metadata to log server */
	char* rowId = SPI_getvalue(row, rel->rd_att, idFieldNumber);
	char* hash = SPI_getvalue(row, rel->rd_att, hashchainFieldNumber);
	syslog(LOG_INFO, "%s %s %s", tableName, rowId, hash);
}
