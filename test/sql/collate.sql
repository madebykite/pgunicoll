-- Warning: Older ICU versions (before 4.8 or 4.6) may give different results
CREATE EXTENSION pgunicoll;

SELECT * FROM (VALUES ('ÂA'), ('ÂB'), ('ÂC'), ('AA'), ('AB'), ('AC')) AS x(txt)
    ORDER BY pgunicoll(txt, '');
SELECT * FROM (VALUES ('AAA'), ('äää'), ('ÄÄÄ'), ('aaa'), ('z'), ('Z'), ('100'), ('2')) AS x(txt)
    ORDER BY pgunicoll(txt, '');
SELECT * FROM (VALUES ('z'), ('ö')) AS x(txt)
    ORDER BY pgunicoll(txt, 'de');
SELECT * FROM (VALUES ('z'), ('ö')) AS x(txt)
    ORDER BY pgunicoll(txt, 'se');
SELECT * FROM (VALUES ('of'), ('öf')) AS x(txt)
    ORDER BY pgunicoll(txt, 'de');
SELECT * FROM (VALUES ('of'), ('öf')) AS x(txt)
    ORDER BY pgunicoll(txt, 'de@collation=phonebook');
SELECT pgunicoll('foobar');
SELECT pgunicoll('ľščťžýáí');
SELECT pgunicoll('');
SELECT pgunicoll(null);
