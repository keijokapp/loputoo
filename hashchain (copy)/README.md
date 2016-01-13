PostgreSQL trigger, mis räsiaheldab sisestatavaid kirjeid ja/või
saadab tulemuse syslog serverile kujul "tabeli_nimi rea_id hash eelmine_hash"
või kujul "tabeli_nimi rea_id hash" sõltuvalt sellest, kas trigger arvutab räsid
ise või mitte.

Repos olev *hashchain-trigger.so* on kompileeritud PostgreSQL 9.4 jaoks
Ubuntu 14.04 x64 masinal. See saadab ainult räsid syslog'i ilma neid ise
arvutamata.

### Kompileerimine

Räsiaheldamine ilma hashide cachimiseta

    make default 

Räsiaheldamine hashide cachimisega

    make use_cache

Ainult räside saatmine serverisse

    make log_only

#### Triggeri seadistamine

Stored procedure loomine

    CREATE FUNCTION "hashchainTrigger"() RETURNS trigger
    	AS '/path/to/hashchain-trigger.so'
    	LANGUAGE C;

Räsiaheldamise trigger

    -- tabel peab sisaldama välja hashchain, mis on piisava suurusega
    -- et räsi salvestada
    CREATE TRIGGER hashchain BEFORE INSERT ON tabeli_nimi
    	FOR EACH ROW EXECUTE PROCEDURE "hashchainTrigger"();

Ainult räside saatmine

    -- tabel peab sisaldama välja hashchain
    CREATE TRIGGER hashchain AFTER INSERT ON tabeli_nimi
    	FOR EACH ROW EXECUTE PROCEDURE "hashchainTrigger"();

