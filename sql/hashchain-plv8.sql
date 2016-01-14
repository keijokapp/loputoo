
CREATE OR REPLACE FUNCTION test_trigger() RETURNS trigger AS
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


DROP TRIGGER if exists identity_hashchain_trigger on identity;
DROP TRIGGER if exists petition_hashchain_trigger on petition;
DROP TRIGGER if exists document_hashchain_trigger on document;
DROP TRIGGER if exists decision_hashchain_trigger on decision;

CREATE TRIGGER identity_test_trigger BEFORE INSERT ON identity_test FOR EACH ROW EXECUTE PROCEDURE test_trigger();
CREATE TRIGGER petition_test_trigger BEFORE INSERT ON petition_test FOR EACH ROW EXECUTE PROCEDURE test_trigger();
CREATE TRIGGER document_test_trigger BEFORE INSERT ON document_test FOR EACH ROW EXECUTE PROCEDURE test_trigger();
CREATE TRIGGER decision_test_trigger BEFORE INSERT ON decision_test FOR EACH ROW EXECUTE PROCEDURE test_trigger();


