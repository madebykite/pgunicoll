CREATE OR REPLACE FUNCTION
  pgunicoll(TEXT)
RETURNS
  BYTEA
AS
  'pgunicoll'
LANGUAGE
  C
STRICT
IMMUTABLE;
