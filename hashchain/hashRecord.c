#include <string.h>
#include <syslog.h>
#include <openssl/sha.h>

#include <postgres.h>
#include <executor/spi.h>
#include <utils/rel.h>
#include <utils/builtins.h>

#include "hashRecord.h"
#ifdef CACHE_LAST_HASH
#include "hashtable.h" // not needed if hash caching is disabled
#endif

static const char hexdigits[16] = "0123456789abcdef";
static const char hashchainFieldName[] = "hashchain";

#ifdef CACHE_LAST_HASH
static struct ht_hashtable* lastHashCache = NULL;
#endif

/**
 * loads & returns hash of previous row of the relation
 * if CACHE_LAST_HASH is defined, this should not be executed more
 * than once per table per server startup.
 * @param relationName name of relation/table
 * @returns hashchain value of last row of the relation
 * @note returned pointer points to statically allocated memory
 */
const char* loadHash(const char relationName[NAMEDATALEN]) {
	static const char prevHashQueryTemplate[] = "SELECT hashchain FROM \"%s\" ORDER BY id DESC LIMIT 1";
	// hash of some random string ("PahGoo3z" in that case)
	static const char initialHash[] = "160488245c65a2f36685f107e1f499caac7f37dc6afb65a4ed9cc9f300b30f05";
	static char retHash[SHA256_DIGEST_LENGTH * 2 + 1] = "";
	static char query[NAMEDATALEN + sizeof(prevHashQueryTemplate)] = ""; // NAMEDATALEN is macro defined by postgre
	
	syslog(LOG_INFO, "Trying to load hash from \"%s\"", relationName);

	snprintf(query, sizeof(query), prevHashQueryTemplate, relationName);

	if(SPI_connect() == SPI_ERROR_CONNECT) {
		syslog(LOG_ERR, "Failed to load hash from \"%s\": SPI_connect() == SPI_ERROR_CONNECT", relationName);
		return NULL;
	}

	int r =	SPI_execute(query, true, 0);

	if(r != SPI_OK_SELECT) {
		syslog(LOG_ERR, "Failed to load hash from \"%s\": r != SPI_OK_SELECT", relationName);
		SPI_finish(); // ignore SPI_ERROR_UNCONNECTED error
		return NULL;
	}

	if(SPI_processed != 1) {
		SPI_finish(); // ignore SPI_ERROR_UNCONNECTED error
		syslog(LOG_INFO, "No rows returned from  \"%s\", using initial hash: %s", relationName, initialHash);
		return initialHash;
	}

	HeapTuple row = SPI_tuptable->vals[0];
	TupleDesc desc = SPI_tuptable->tupdesc;

	char* hash = SPI_getvalue(row, desc, 1);
	memcpy(retHash, hash, SHA256_DIGEST_LENGTH * 2 + 1);
	pfree(hash);
	
	SPI_finish(); // ignore SPI_ERROR_UNCONNECTED error

	syslog(LOG_INFO, "Hash loaded from \"%s\": %s", relationName, retHash);

	return retHash;
}

/**
 * TODO error checking? (sha256 functions)
 * @param rel relation in question
 * @param row row to be inserted
 * @returns row with hashchain field filled
 * @note Postgre server takes care of freeing allocated memory
 * @note elog statements seem to have significant performance impact
 */
HeapTuple hashRecord(const Relation rel, const HeapTuple row) {
	const char* tableName = SPI_getrelname(rel); // table name
	int hashchainFieldNumber = SPI_fnumber(rel->rd_att, hashchainFieldName); // hashchain field index
	int idFieldNumber = SPI_fnumber(rel->rd_att, "id"); // current row id

	if(hashchainFieldNumber <= 0) {
		elog(NOTICE, "hashchain triggered on table without hashchain field (%s)", tableName);
		return NULL; // not hashchain'ed, no-op
	}

#ifdef CACHE_LAST_HASH
	if(lastHashCache == NULL) lastHashCache = ht_hashtable_new(31);
	const char* prevHash = ht_hashtable_get(lastHashCache, tableName);
	if(prevHash == NULL) prevHash = loadHash(tableName);
#else
	const char* prevHash = loadHash(tableName);
#endif

	if(prevHash == NULL) {
		elog(ERROR, "Couldn't get last hash (see system log)");
		return NULL;
	}
//	elog(INFO, "Previous hash: %s", prevHash);


	/* initialize sha256 context */
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, prevHash, SHA256_DIGEST_LENGTH * 2);

	/* add all fields to digest */
	int i;
	for(i = 1; i <= rel->rd_att->natts; i++) {
		if(i == hashchainFieldNumber) continue; // except hashchain field

		const char* fieldValue = SPI_getvalue(row, rel->rd_att, i);
//		elog(INFO, "adding field \"%s\" to hash: %s (%d)",
//			SPI_fname(rel->rd_att, i),
//			fieldValue,
//			(int)strlen(fieldValue));
			
		SHA256_Update(&sha256, fieldValue, strlen(fieldValue));
	}


	/* first 32 bytes of output will be binary representation of digest */
	unsigned char hash[SHA256_DIGEST_LENGTH * 2 + 1];
	SHA256_Final(hash, &sha256);

	/* convert binary representation to string */
	for(i = SHA256_DIGEST_LENGTH - 1; i >= 0; i--) {
		hash[i * 2 + 1] = hexdigits[hash[i] & 0x0f];
		hash[i * 2] = hexdigits[(hash[i] & 0xf0) >> 4];
	}
	hash[SHA256_DIGEST_LENGTH * 2] = '\0'; // string terminator
//	elog(INFO, "Final hash: %s", hash);


	/* send hash & metadata to log server */
	char* rowId = SPI_getvalue(row, rel->rd_att, idFieldNumber);
	syslog(LOG_INFO, "%s %s %s %s", tableName, rowId, prevHash, hash);

#ifdef CACHE_LAST_HASH
	/* save hash to cache (variable 'hash' will be strdup'ed) */
	ht_hashtable_set(lastHashCache, tableName, hash);
#endif

	/* create new tuple with hash */
	int columns[1] = { hashchainFieldNumber };
	Datum values[1] = { CStringGetTextDatum(hash) };
	HeapTuple modifiedTuple = SPI_modifytuple(rel, row, 1, columns, values, NULL);

	if(SPI_result == SPI_ERROR_NOATTRIBUTE) {
		elog(ERROR, "SPI_result == SPI_ERROR_NOATTRIBUTE");
		return 0;	
	}

	return modifiedTuple;
}
