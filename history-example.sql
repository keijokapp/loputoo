
-- rekursiivne viitamine
﻿DROP TABLE IF EXISTS history_test;
CREATE table history_test(
	id BIGSERIAL PRIMARY KEY,
	previous_revision BIGINT REFERENCES history_test(id) UNIQUE
)

INSERT INTO history_test(previous_revision) VALUES(0);
SELECT * FROM history_test;

-- kirjete sidumine identifikaatoriga
﻿DROP TABLE IF EXISTS history_test;
CREATE table history_test(
	id BIGSERIAL PRIMARY KEY,
	object_uuid VARCHAR(36)
)

INSERT INTO history_test(object_uuid) VALUES('');
SELECT * FROM history_test;
