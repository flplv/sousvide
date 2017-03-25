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
#include <reacto/reusables/array.h>

void array_init(array_t * obj, size_t max_length)
{
    debug_ptr(obj);

    obj->max = max_length;
    obj->top = 0;
}

void array_deinit(array_t * obj)
{
    debug_ptr(obj);

    obj->max = 0;
    obj->top = 0;
}

void array_clear(array_t * obj)
{
    debug_ptr(obj);
    obj->top = 0;
}

size_t array_length(array_t * obj)
{
    debug_ptr(obj, 0);
    return obj->top;
}

bool array_full(array_t * obj)
{
    debug_ptr(obj, true);
    return obj->top == obj->max;
}
