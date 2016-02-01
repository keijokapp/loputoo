DROP TABLE IF EXISTS history_test;
CREATE table history_test(
	id BIGSERIAL PRIMARY KEY,
	previous_revision BIGINT REFERENCES history_test(id) UNIQUE
)

INSERT INTO history_test(previous_revision) VALUES(0);
SELECT * FROM history_test;
