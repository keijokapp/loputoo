default:
	gcc trigger.c hashRecord.c -I/usr/include/postgresql/9.4/server -lpq -lcrypto -shared -fPIC -o hashchainTrigger.so
cached_hashes:
	gcc trigger.c hashRecord.c hashtable.c -DCACHE_LAST_HASH -I/usr/include/postgresql/9.4/server -lpq -lcrypto -shared -fPIC -o hashchainTrigger.so
ship_hash_only:
	gcc trigger.c shipHash.c -DSHIP_HASH_ONLY -I/usr/include/postgresql/9.4/server -lpq -shared -fPIC -o hashchainTrigger.so
