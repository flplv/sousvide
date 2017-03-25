/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Felipe Lavratti
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef REACTO_REUSABLES_TIME_H_
#define REACTO_REUSABLES_TIME_H_

#include <stdint.h>

/* reacto_time_t is the type used to measure time.
 * - it must be unsigned
 * - it must be big enough to store the amount of time required
 *    by the application considering the platform provided time
 *    unit.
 * REACTO_TIME_TYPE macro can be used to change the time unit in
 *  built time. Be aware you can cause ABI mismatch if you build
 *  the react.o library with a different setting than the one
 *  used in the application.
 */
#ifndef REACTO_TIME_TYPE
typedef unsigned long reacto_time_t;
#else
typedef REACTO_TIME_TYPE reacto_time_t;
#endif

/*
 * Get current platform time in platform time unit
 */
reacto_time_t time_now(void);

#endif /* REACTO_REUSABLES_TIME_H_ */
