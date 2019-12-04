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

#include <assert.h>

#include "base64.h"


static const char ENC_LUT[64] =
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


void base64_encoder_init(base64_encoder_t* encoder, const base64_encoder_options_t* options, void* user_data)
{
    assert(encoder);
    assert(options);

    encoder->options = options;
    encoder->user_data = user_data;
    encoder->n_pending = 0;
    encoder->line_length = 0;
}


int base64_encoder_write(base64_encoder_t* encoder, const void* buffer, size_t length)
{
    assert(encoder);
    assert(buffer || length == 0);

    int result;
    const uint8_t* in_i = buffer;
    const uint8_t* in_e = in_i + length;
    uint8_t* pending = encoder->pending;
    char out[5];

    out[4] = '\n';

    while (in_i < in_e)
    {
        uint8_t byte = *in_i++;

        pending[encoder->n_pending++] = byte;

        if ( encoder->n_pending == 3 )
        {
            unsigned int out_len = 4;

            if ( encoder->options->max_line_length > 0 )
            {
                encoder->line_length += 4;

                if ( encoder->line_length >= encoder->options->max_line_length )
                {
                    encoder->line_length = 0;
                    out_len = 5;
                }
            }

            out[0] = ENC_LUT[pending[0] >> 2u];
            out[1] = ENC_LUT[((pending[0] & 0x03u) << 4u) | (pending[1] >> 4u)];
            out[2] = ENC_LUT[((pending[1] & 0x0fu) << 2u) | (pending[2] >> 6u)];
            out[3] = ENC_LUT[pending[2] & 0x3Fu];

            result = encoder->options->write_func(out, out_len, encoder->user_data);
            if ( result < 0 ) return result;

            encoder->n_pending = 0;
        }
    }

    return 0;
}


int base64_encoder_finish(base64_encoder_t* encoder)
{
    assert(encoder);

    uint8_t* pending = encoder->pending;
    char out[6];
    char* out_i = out;

    if ( encoder->n_pending > 0 )
    {
        *out_i++ = ENC_LUT[pending[0] >> 2u];

        if ( encoder->n_pending == 1 )
        {
            *out_i++ = ENC_LUT[(pending[0] & 0x03) << 4];
            *out_i++ = '=';
        }
        else
        {
            *out_i++ = ENC_LUT[((pending[0] & 0x03u) << 4u) | (pending[1] >> 4u)];
            *out_i++ = ENC_LUT[(pending[1] & 0x0Fu) << 2u];
        }

        *out_i++ = '=';

        encoder->line_length += 4;
    }

    if ( (encoder->options->max_line_length > 0) && (encoder->line_length > 0) ) *out_i++ = '\n';
    if ( encoder->options->null_terminate ) *out_i++ = '\0';

    return encoder->options->write_func(out, out_i - out, encoder->user_data);
}


static const unsigned char DEC_LUT[256] =
{
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
    0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0x80, 0xFF, 0xFF,
    0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
    0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
    0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
    0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};


void base64_decoder_init(base64_decoder_t* decoder, const base64_decoder_options_t* options, void* user_data)
{
    assert(decoder);
    assert(options);

    decoder->options = options;
    decoder->user_data = user_data;
    decoder->n_pending = 0;
}


int base64_decoder_write(base64_decoder_t* decoder, const void* buffer, size_t length)
{
    assert(decoder);
    assert(buffer || length == 0);

    int result;
    const uint8_t* in_i = buffer;
    const uint8_t* in_e = in_i + length;
    uint8_t* pending = decoder->pending;
    uint8_t out[3];


    while (in_i < in_e)
    {
        uint8_t byte = DEC_LUT[*in_i++];

        if ( byte == 0xFF ) continue;

        pending[decoder->n_pending++] = byte;

        if ( decoder->n_pending == 4 )
        {
            unsigned int out_len;

            if ( pending[2] == 0x80 )
            {
                out_len = 1;
                pending[2] = 0x00;
                pending[3] = 0x00;
            }
            else if ( pending[3] == 0x80 )
            {
                out_len = 2;
                pending[3] = 0x00;
            }
            else
            {
                out_len = 3;
            }

            out[0] = (pending[0] << 2u) | (pending[1] >> 4u);
            out[1] = (pending[1] << 4u) | (pending[2] >> 2u);
            out[2] = (pending[2] << 6u) | pending[3];

            result = decoder->options->write_func(out, out_len, decoder->user_data);
            if ( result < 0 ) return result;

            decoder->n_pending = 0;
        }
    }

    return BASE64_RESULT_OK;
}


int base64_decoder_finish(base64_decoder_t* decoder)
{
    assert(decoder);
    return (decoder->n_pending > 0) ? BASE64_RESULT_UNEXPECTED_END_OF_INPUT : BASE64_RESULT_OK;
}
