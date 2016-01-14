
CREATE OR REPLACE FUNCTION hashchainTrigger() RETURNS trigger AS
$$
	const tableName = '"' + TG_TABLE_SCHEMA + '"."' + TG_TABLE_NAME + '"';
	var q = plv8.execute('SELECT hashchain FROM ' + tableName + ' ORDER BY id DESC LIMIT 1');
	plv8.elog(NOTICE, 'previous hash:', q[0].hashchain);
	NEW.hashchain = q[0].hashchain;
	var row = JSON.stringify(NEW);
	NEW.hashchain = plv8.sha256(row); //plv8.find_function("sha256")(row);
	return NEW
$$
LANGUAGE "plv8";


DROP TRIGGER if exists hashchainTrigger on hashchainTest;
CREATE TRIGGER hashchainTrigger BEFORE INSERT ON hashchain_test FOR EACH ROW EXECUTE PROCEDURE test_trigger();


