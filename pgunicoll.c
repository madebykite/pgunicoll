#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unicode/utypes.h>
#include <unicode/ucol.h>
#include <unicode/ustring.h>

#include "postgres.h"
#include "fmgr.h"

PG_MODULE_MAGIC;

Datum pgunicoll (PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(pgunicoll);

#define PREALLOC_SIZE 256

static int
icu_failure (UErrorCode status)
{
    if (U_SUCCESS(status))
        return 0;

    ereport(ERROR, (errmsg("ICU error: %s", u_errorName (status))));
    return 1;
}

static UChar *
unicode_from_pg_text (text *pg_input)
{
    UErrorCode status = U_ZERO_ERROR;

    char *input = VARDATA (pg_input);
    int32_t len = VARSIZE (pg_input) - VARHDRSZ;

    UChar *ret = (UChar *) palloc (sizeof (UChar) * PREALLOC_SIZE);
    int32_t size;

    u_strFromUTF8WithSub (ret, PREALLOC_SIZE, &size, input, len, 0xFFFD, NULL, &status);

    /* always allocate 1 character more than neccesary, this ensures that
     * u_strFromUTF8WithSub() will write a zero-terminated string. */
    if (++size > PREALLOC_SIZE)
    {
        pfree (ret);
        ret = (UChar *) palloc (sizeof (UChar) * size);

        status = U_ZERO_ERROR;
        u_strFromUTF8WithSub (ret, size, NULL, input, len, 0xFFFD, NULL, &status);
    }

    if (icu_failure (status))
    {
        pfree (ret);
        return NULL;
    }

    return ret;
}

static int32_t
sortkey_from_unicode (UChar *input, uint8_t **output)
{
    UErrorCode status = U_ZERO_ERROR;
    UCollator * collator = ucol_open ("", &status);
    int32_t size;

    if (icu_failure (status))
        return 0;

    ucol_setAttribute (collator, UCOL_NUMERIC_COLLATION, UCOL_ON, &status);

    if (icu_failure (status))
        return 0;

    *output = (uint8_t *) palloc (sizeof (uint8_t) * PREALLOC_SIZE);
    size = ucol_getSortKey (collator, input, -1, *output, PREALLOC_SIZE);

    if (size > PREALLOC_SIZE)
    {
        pfree (*output);
        *output = (uint8_t *) palloc (sizeof (uint8_t) * size);
        ucol_getSortKey (collator, input, -1, *output, size);
    }

    ucol_close (collator);

    if (size < 1)
    {
        ereport(ERROR, (errmsg("ICU sortkey is zero")));
    }

    return size;
}

Datum
pgunicoll (PG_FUNCTION_ARGS)
{
    UChar *unicode;
    uint8_t *sortkey = NULL;
    int32_t sortkeylen;
    bytea *output;

    if (PG_ARGISNULL (0))
    {
        PG_RETURN_NULL();
    }

    unicode = unicode_from_pg_text (PG_GETARG_TEXT_P(0));
    if (!unicode)
    {
        PG_RETURN_NULL();
    }

    sortkeylen = sortkey_from_unicode (unicode, &sortkey);
    if (!sortkeylen)
    {
        PG_RETURN_NULL();
    }

    output = (bytea *)palloc (sortkeylen + VARHDRSZ);

    SET_VARSIZE (output, sortkeylen + VARHDRSZ);

    memcpy (VARDATA (output), sortkey, sortkeylen);

    pfree (unicode);
    pfree (sortkey);

    PG_RETURN_BYTEA_P( output );
}
