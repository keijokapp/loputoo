DROP TABLE IF EXISTS history_test;
CREATE table history_test(
	id BIGSERIAL PRIMARY KEY,
	object_uuid VARCHAR(36)
)

INSERT INTO history_test(object_uuid) VALUES('');
SELECT * FROM history_test;
