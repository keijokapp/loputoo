default:
	gcc trigger.c hashchain.c logHash.c -I/usr/include/postgresql/9.4/server -lpq -lcrypto -shared -fPIC -o hashchain-trigger.so
	chmod 755 hashchain-trigger.so
	
