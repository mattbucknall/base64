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

#include "base64.h"


size_t base64_buffer_size_to_raw_buffer_size(size_t size)
{
    // TODO!
    return 0;
}


size_t base64_buffer_size_from_raw_buffer_size(size_t size, const base64_options_t* options)
{
    size *= 8;
    size = (size + 5) / 6;
    size = (size + 3) / 4;
    size *= 4;

    if ( options )
    {
        if ( options->with_lines ) size += (size + 71) / 72;
        if ( options->null_terminate ) size += 1;
    }

    return size;
}


base64_result_t base64_to_raw(const void* base64_buffer, size_t base64_size, void* raw_buffer, size_t raw_size)
{
    // TODO!
    return BASE64_RESULT_OK;
}


base64_result_t base64_from_raw(void* base64_buffer, size_t base64_size,
        const void* raw_buffer, size_t raw_size, const base64_options_t* options)
{
    static const unsigned char LUT[64] =
    {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/'
    };

    const unsigned char* in_i;
    const unsigned char* in_e;
    unsigned char* out_i;
    unsigned line_length;
    int with_lines = options ? options->with_lines : 0;
    int null_terminate = options ? options->null_terminate : 0;

    if ( base64_size < base64_buffer_size_from_raw_buffer_size(raw_size, options) ) return BASE64_RESULT_BUFFER_TOO_SMALL;

    in_i = (const unsigned char*) raw_buffer;
    in_e = in_i + raw_size;
    out_i = (unsigned char*) base64_buffer;
    line_length = 0;

    while (in_e - in_i >= 3)
    {
        *out_i++ = LUT[in_i[0] >> 2u];
        *out_i++ = LUT[((in_i[0] & 0x03u) << 4u) | (in_i[1] >> 4u)];
        *out_i++ = LUT[((in_i[1] & 0x0fu) << 2u) | (in_i[2] >> 6u)];
        *out_i++ = LUT[in_i[2] & 0x3Fu];

        in_i += 3;

        if ( with_lines )
        {
            line_length += 4;

            if (line_length >= 72)
            {
                line_length = 0;
                *out_i++ = '\n';
            }
        }
    }

    if (in_i < in_e)
    {
        *out_i++ = LUT[in_i[0] >> 2u];

        if ( (in_e - in_i) == 1 )
        {
            *out_i++ = LUT[(in_i[0] & 0x03u) << 4u];
            *out_i++ ='=';
        }
        else
        {
            *out_i++ = LUT[((in_i[0] & 0x03u) << 4u) | (in_i[1] >> 4u)];
            *out_i++ = LUT[(in_i[1] & 0x0Fu) << 2u];
        }

        *out_i++ = '=';
        line_length += 4;
    }

    if ( with_lines && line_length > 0 ) *out_i++ = '\n';
    if ( null_terminate ) *out_i = '\0';
}
