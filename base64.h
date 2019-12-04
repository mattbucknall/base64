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
#include <stdint.h>


#define BASE64_RESULT_OK                         0
#define BASE64_RESULT_UNEXPECTED_END_OF_INPUT   -1
#define BASE64_RESULT_FIRST_USER_CODE           -100


typedef int (*base64_write_func_t) (const void* buffer, size_t length, void* user_data);


typedef struct
{
    base64_write_func_t write_func;
     unsigned int max_line_length;
     int null_terminate;

} base64_encoder_options_t;


typedef struct
{
    const base64_encoder_options_t* options;
    void* user_data;
    uint8_t pending[3];
    int n_pending;
    unsigned int line_length;

} base64_encoder_t;


typedef struct
{
    base64_write_func_t write_func;

} base64_decoder_options_t;


typedef struct
{
    const base64_decoder_options_t* options;
    void* user_data;
    uint8_t pending[4];
    int n_pending;

} base64_decoder_t;


void base64_encoder_init(base64_encoder_t* encoder, const base64_encoder_options_t* options, void* user_data);

int base64_encoder_write(base64_encoder_t* encoder, const void* buffer, size_t length);

int base64_encoder_finish(base64_encoder_t* encoder);

void base64_decoder_init(base64_decoder_t* decoder, const base64_decoder_options_t* options, void* user_data);

int base64_decoder_write(base64_decoder_t* decoder, const void* buffer, size_t length);

int base64_decoder_finish(base64_decoder_t* decoder);

#endif /* _BASE64_H_ */
