SELECT  * FROM application WHERE data->'identity'->'firstname' @> CAST ('"Deniss"' as jsonb) AND data->'identity'->'lastname' @> CAST ('"Roos"' as jsonb)