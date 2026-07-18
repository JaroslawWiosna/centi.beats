# centi.beats

```console
$ make 
gcc -static -O3 -g main.c -o centi.beats
./centi.beats
21:31:14 @896.70[CEST] - 19:31:14 @813.36[UTC] - centi.beats  @855.03
```

## Further reading:

### https://en.wikipedia.org/wiki/Swatch_Internet_Time
### https://en.wikipedia.org/wiki/Decimal_time#France


```
/*
 * centi.beats
 *
 * Summary:
 *   Continuously prints a single terminal line with three 24-hour clocks and
 *   their corresponding `local centi.beats` values:
 *
 *   1) Local wall-clock time (HH:MM:SS, 00..23) and `local centi.beats`, where 
 *      the beat value is computed from time-of-day mapped onto 
 *      a 0..1000 timeline over exactly 24 hours.
 *
 *   2) UTC wall-clock time (HH:MM:SS, 00..23) and `centi.beats`
 *      on the same 0..1000 scale.
 *
 *   3) A CET-like reference: UTC shifted by +1 hour (UTC + 3600s), then
 *      converted to `centi.beats` using the same 0..1000 timeline.
 *
 * Output Example:
 *   ```console
 *   $ gcc -static -O3 -g main.c -o centi.beats && ./centi.beats
 *   21:13:08 @884.12[CEST] - 19:13:08 @800.79[UTC] - centi.beats  @842.46
 *   ```
 * 
 * Centi-beats:
 *   beats = (seconds_since_midnight / 86400) * 1000.0 always for CET (UTC+1h)
 *   where seconds_since_midnight includes the current millisecond from 
 *   CLOCK_REALTIME.
 *
 *   The beat scale spans one full 24-hour day:
 *     00:00:00.000 -> @000.00
 *     23:59:59.999 -> @999.99  (never @1000.00; wrap corresponds 
 *                                               conceptually to 000 next day)
 * Local centi.beats:
 *   Vanilla beats is for CET (UTC+1h), but 
 *   local beats is for your local timezone, 
 *   and is labelled by square brackets suffix
 *
 * Implementation overview:
 *   - Reads current time via clock_gettime(CLOCK_REALTIME) and
 *     extracts milliseconds from tv_nsec.
 *   - Updates tm_* only once per second (when tv_sec changes).
 *   - Converts tm_* to beats with tm2clock_and_localbeat().
 *
 * Further reading:
 *   - https://en.wikipedia.org/wiki/Swatch_Internet_Time
 *   - https://en.wikipedia.org/wiki/Decimal_time#France
 *
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Jarosław Wiosna ( https://github.com/JaroslawWiosna )
 */
/*
 * centi.beats
 *
 * Summary:
 *   Continuously prints a single terminal line with three 24-hour clocks and
 *   their corresponding `local centi.beats` values:
 *
 *   1) Local wall-clock time (HH:MM:SS, 00..23) and `local centi.beats`, where 
 *      the beat value is computed from time-of-day mapped onto 
 *      a 0..1000 timeline over exactly 24 hours.
 *
 *   2) UTC wall-clock time (HH:MM:SS, 00..23) and `centi.beats`
 *      on the same 0..1000 scale.
 *
 *   3) A CET-like reference: UTC shifted by +1 hour (UTC + 3600s), then
 *      converted to `centi.beats` using the same 0..1000 timeline.
 *
 * Output Example:
 *   ```console
 *   $ gcc -static -O3 -g main.c -o centi.beats && ./centi.beats
 *   21:13:08 @884.12[CEST] - 19:13:08 @800.79[UTC] - centi.beats  @842.46
 *   ```
 * 
 * Centi-beats:
 *   beats = (seconds_since_midnight / 86400) * 1000.0 always for CET (UTC+1h)
 *   where seconds_since_midnight includes the current millisecond from 
 *   CLOCK_REALTIME.
 *
 *   The beat scale spans one full 24-hour day:
 *     00:00:00.000 -> @000.00
 *     23:59:59.999 -> @999.99  (never @1000.00; wrap corresponds 
 *                                               conceptually to 000 next day)
 * Local centi.beats:
 *   Vanilla beats is for CET (UTC+1h), but 
 *   local beats is for your local timezone, 
 *   and is labelled by square brackets suffix
 *
 * Implementation overview:
 *   - Reads current time via clock_gettime(CLOCK_REALTIME) and
 *     extracts milliseconds from tv_nsec.
 *   - Updates tm_* only once per second (when tv_sec changes).
 *   - Converts tm_* to beats with tm2clock_and_localbeat().
 *
 * Further reading:
 *   - https://en.wikipedia.org/wiki/Swatch_Internet_Time
 *   - https://en.wikipedia.org/wiki/Decimal_time#France
 *
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Jarosław Wiosna ( https://github.com/JaroslawWiosna )
 */
```

