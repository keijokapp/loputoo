default:
	gcc trigger.c hashRecord.c -I/usr/include/postgresql/9.4/server -lpq -lcrypto -shared -fPIC -o hashchainTrigger.so
use_cache:
	gcc trigger.c hashRecord.c hashtable.c -DUSE_CACHE -I/usr/include/postgresql/9.4/server -lpq -lcrypto -shared -fPIC -o hashchainTrigger.so
log_only:
	gcc trigger.c logHash.c -DLOG_ONLY -I/usr/include/postgresql/9.4/server -lpq -shared -fPIC -o hashchainTrigger.so
