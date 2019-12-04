/*
 * Base64 Encoder/Decoder C Routines
 * <https://github.com/mattbucknall/json>
 *
 * Copyright (c) 2019 Matthew T. Bucknall
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef _BASE64_H_
#define _BASE64_H_

#include <stddef.h>


typedef enum
{
    BASE64_RESULT_OK,
    BASE64_RESULT_BUFFER_TOO_SMALL,
    BASE64_RESULT_INVALID_CHARACTER

} base64_result_t;


typedef struct
{
     int with_lines;
     int null_terminate;

} base64_options_t;


size_t base64_buffer_size_to_raw_buffer_size(size_t size);

size_t base64_buffer_size_from_raw_buffer_size(size_t size, const base64_options_t* options);

base64_result_t base64_to_raw(const void* base64_buffer, size_t base64_size, void* raw_buffer, size_t raw_size);

base64_result_t base64_from_raw(void* base64_buffer, size_t base64_size,
        const void* raw_buffer, size_t raw_size,
        const base64_options_t* options);

#endif /* _BASE64_H_ */
