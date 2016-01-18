DROP TABLE IF EXISTS hashchain_test;
CREATE TABLE hashchain_test (
	id BIGSERIAL,
	data TEXT,
	hashchain VARCHAR(255)
);


-- Funktsioonide registeerimine

-- krüptoaheldamise funktsioon
CREATE OR REPLACE FUNCTION "hashchainTrigger"()		-- funktsiooni nimeks on 'hashchainTrigger'
	RETURNS trigger 			-- funktsiooni kasutatakse päästiku jaoks
	AS '/var/local/hashchain-trigger.so'	-- kompileerimise väljundfail 
	LANGUAGE C;				-- funktsioon on kirjutatud C keeles

-- Räsi logisse saatmise funktsioon
CREATE OR REPLACE FUNCTION "logHashTrigger"()		-- funktsiooni nimeks on 'logHashTrigger'
	RETURNS trigger 			-- funktsiooni kasutatakse päästiku jaoks
	AS '/var/local/hashchain-trigger.so'	-- kompileerimise väljundfail 
	LANGUAGE C;				-- funktsioon on kirjutatud C keeles

-- Päästikute registreerimine

-- Päästik, mis krüptoaheldab kirjed
CREATE TRIGGER hashchainTrigger			-- päästiku nimi
	BEFORE INSERT					-- enne sisestamist
	ON hashchain_test					-- tabeli nimi
	FOR EACH ROW					-- iga rea jaoks eraldi 
	EXECUTE PROCEDURE "hashchainTrigger"(); 	-- funktsiooni nimi

-- Päästik, mis kirjed logisse saadab	
CREATE TRIGGER logHashTrigger				-- päästiku nimi
	BEFORE INSERT					-- enne sisestamist
	ON hashchain_test					-- tabeli nimi
	FOR EACH ROW					-- iga rea jaoks eraldi
	EXECUTE PROCEDURE "logHashTrigger"();	-- funktsiooni nimi

insert into hashchain_test (data) values('Lorem ipsum dolor sit amet, consectetur adipiscing elit.');
select * from hashchain_test