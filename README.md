PostgreSQL Unicode Collation Extension
======================================

This extension provides collation support using the
[Unicode Collation Algorithm](http://www.unicode.org/unicode/reports/tr10/),
as it is implemented by the
[International Components for Unicode](http://userguide.icu-project.org/collation)
library.


Usage
-----

This module provides a simple function to generate a sortkey from a postgresql
TEXT column:

    bytea pgunicoll(text subject, [text locale])

The locale indicates the required collation rules. Special values for locales can be passed in - see the [ICU collation API user guide](http://userguide.icu-project.org/collation/api) for details. If empty string ("") or "root" are passed, the root collator will be returned.

    test=> select * from unsorted order by pgunicoll(column) limit 4;
     name
    ------
     AAA
     ÄÄÄ
     aaa
     äää

Note that older versions of ICU may give a different order:

     name
    ------
     aaa
     AAA
     äää
     ÄÄÄ

Warning: The function always assumes that input is UTF-8 encoded. It is not very useful
in databases with other encodings.

Warning: Indexes based on this function should be rebuilt after upgrading ICU.


Requirements
------------

To use this extension you will need:

- PostgreSQL version 8.3 or newer
- libicu version 3.8 or newer

You will need -dev packages installed for both of those, check that pg_config
and icu-config are in your path.


Installation
------------

To build and install this extension, simply run:

    % make
    % sudo make install

Then, to activate this extension in your database, run the SQL:

    CREATE EXTENSION pgunicoll;

Databases using the old extension system can be upgraded with:

    CREATE EXTENSION pgunicoll FROM unpackaged;

If you run into problems with building, see [PostgreSQL wiki for
troubleshooting](https://wiki.postgresql.org/wiki/Extension_build_troubleshooting)


License
-------

pgunicoll, a postgresql extension to sort with the UCA. This extension is heavily based on the MetaBrainz foundation [postgresql-musicbrainz-collate](https://github.com/metabrainz/postgresql-musicbrainz-collate) extension.

* Copyright 2010  MetaBrainz Foundation
* Copyright 2016  Kite Development & Consulting Ltd

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
