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

#include <stdio.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    int hour;
    int min;
    int sec;
    double beats;
} Clock_and_Localbeat;

static Clock_and_Localbeat tm2clock_and_localbeat(const struct tm *tm, int ms)
{
    double seconds_since_midnight =
        tm->tm_hour * 3600.0 + tm->tm_min * 60.0 + tm->tm_sec + ms / 1000.0;

    double beats = (seconds_since_midnight / 86400.0) * 1000.0;

    return (Clock_and_Localbeat){
        .hour = tm->tm_hour,
        .min  = tm->tm_min,
        .sec  = tm->tm_sec,
        .beats = beats,
    };
}

int main(void)
{
    struct timespec ts;

    time_t last_sec = (time_t)-1;
    struct tm tm_local, tm_utc, tm_cet;
    char tz_buf[64] = {0};

    for (;;) {
        clock_gettime(CLOCK_REALTIME, &ts);
        time_t sec = ts.tv_sec;
        int ms = (int)(ts.tv_nsec / 1000000);

        if (sec != last_sec) {
            last_sec = sec;

            localtime_r(&sec, &tm_local);
            gmtime_r(&sec, &tm_utc);

            time_t sec_cet = sec + 3600;
            gmtime_r(&sec_cet, &tm_cet);

            strftime(tz_buf, sizeof(tz_buf), "%Z", &tm_local);
        }

        Clock_and_Localbeat c_local = tm2clock_and_localbeat(&tm_local, ms);
        Clock_and_Localbeat c_utc   = tm2clock_and_localbeat(&tm_utc, ms);
        Clock_and_Localbeat c_cet   = tm2clock_and_localbeat(&tm_cet, ms);

        printf("\r\033[K%02d:%02d:%02d @%3.2f[%s]"
               " - %02d:%02d:%02d @%3.2f[UTC]"
               " - centi.beats  @%3.2f",
               c_local.hour, c_local.min, c_local.sec, c_local.beats, tz_buf,
               c_utc.hour, c_utc.min, c_utc.sec, c_utc.beats,
                                                 c_cet.beats);

        fflush(stdout);
        usleep((100  * 1000) - 1);
    }
}

