CREATE or REPLACE FUNCTION tabeli_nimiHashchainTrigger() RETURNS trigger AS $row_stamp$
	DECLARE
		hash char(128);
		previousHash char(128);
	BEGIN
		-- Eelmise räsi lugemine andmebaasist muutujasse prefiousHash
		SELECT hashchain INTO previousHash FROM hashchain_test ORDER BY id DESC LIMIT 1;
		-- Uue räsi arvutamine muutujasse hash
		SELECT encode(digest(CAST((NEW.*, previousHash) AS text),'sha256'), 'hex')
			INTO hash;
		-- Rea räsiväärtuse välja muutmine vastavalt
		NEW.hashchain := hash;
		-- Tagastatakse uus (muudetud) rida
		RETURN NEW;
	END;
	$row_stamp$ LANGUAGE plpgsql;
	
CREATE TRIGGER hashchain					-- päästiku nimi on “hashchain”
	BEFORE INSERT						-- enne sisestamist
	ON tabeli_nimi						-- tabeli nimi
	FOR EACH ROW						-- iga rea jaoks eraldi
	EXECUTE PROCEDURE tabeli_nimiHashchainTrigger();	-- käivitatav funktsioon
