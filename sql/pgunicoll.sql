CREATE OR REPLACE FUNCTION
  pgunicoll(text, text default '')
RETURNS
  bytea
AS
  'pgunicoll'
LANGUAGE
  C
STRICT
IMMUTABLE;
