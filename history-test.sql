DROP TABLE IF EXISTS history_test0;
CREATE TABLE history_test0 (
	id BIGSERIAL PRIMARY KEY,
	previous BIGINT REFERENCES history_test0(id) UNIQUE
);


SELECT * FROM history_test0;

INSERT INTO history_test0(previous) VALUES(20);

EXPLAIN WITH RECURSIVE history(id, previous) AS (
	SELECT id, previous FROM history_test0 WHERE id=18
	UNION DISTINCT
        SELECT history_test0.id, history_test0.previous FROM history, history_test0
        WHERE history_test0.id = history.previous OR history.id = history_test0.previous
)
SELECT * FROM history;


WITH RECURSIVE t(n) AS (
    VALUES (1)
  UNION ALL
    SELECT n+1 FROM t WHERE n < 100
)
SELECT n FROM t;