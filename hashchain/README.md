PostgreSQL trigger, mis räsiaheldab sisestatavaid kirjeid ja/või
saadab tulemuse syslog serverile kujul "tabeli_nimi rea_id hash"

Repos olev *hashchain-trigger.so* on kompileeritud PostgreSQL 9.4 jaoks
Ubuntu 14.04 x64 masinal.

#### Kompileerimine

    make

#### Triggeri seadistamine

Stored procedure loomine

    CREATE FUNCTION "hashchainTrigger"() RETURNS trigger
    	AS '/path/to/hashchain-trigger.so'
    	LANGUAGE C;

    CREATE FUNCTION "logHashTrigger"() RETURNS trigger
    	AS '/path/to/hashchain-trigger.so'
    	LANGUAGE C;

Räsiaheldamise trigger

    -- tabel peab sisaldama välja hashchain, mis on piisava suurusega
    -- et räsi salvestada
    CREATE TRIGGER hashchainTrigger BEFORE INSERT ON tabeli_nimi
    	FOR EACH ROW EXECUTE PROCEDURE "hashchainTrigger"();


Räside logimise trigger

    -- tabel peab sisaldama välja hashchain, mis on piisava suurusega
    -- et räsi salvestada
    CREATE TRIGGER hashchainTrigger BEFORE INSERT ON tabeli_nimi
    	FOR EACH ROW EXECUTE PROCEDURE "logHashTrigger"();


