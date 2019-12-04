#include <stdio.h>
#include <string.h>

#include <base64.h>


static base64_encoder_t m_encoder;
static base64_decoder_t m_decoder;


static int dec_write_func(const void* buffer, size_t length, void* user_data)
{
    fwrite((const char*) buffer, 1, length, stdout);
    return 0;
}

static int enc_write_func(const void* buffer, size_t length, void* user_data)
{
    return base64_decoder_write(&m_decoder, buffer, length);
}


int main()
{
    static const char* STR =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. In posuere augue eget consectetur ultrices. Suspendisse potenti. Etiam in magna\n"
        "eu nisl iaculis imperdiet non ac neque. Aenean risus augue, mollis non lacinia vel, facilisis cursus tortor. Vivamus pellentesque, risus\n"
        "ut dapibus dictum, arcu ipsum eleifend ligula, vel aliquam eros ex eget neque. Proin felis lorem, posuere at scelerisque ac, blandit\n"
        "sollicitudin odio. Curabitur rutrum, quam nec vulputate facilisis, dui ex malesuada odio, id molestie ante dolor et tortor. Sed iaculis\n"
        "odio eget ante ultricies, eget mattis est imperdiet. Sed dictum metus lacinia elit dapibus consectetur. Praesent lorem velit, faucibus et\n"
        "erat eu, lacinia faucibus nulla. Curabitur et consequat nunc. Sed aliquam pharetra pharetra.\n";

    static const base64_encoder_options_t encoder_options =
    {
        .write_func = enc_write_func,
        .max_line_length = 76,
        .null_terminate = 0
    };

    static const base64_decoder_options_t decoder_options =
    {
        .write_func = dec_write_func
    };

    base64_decoder_init(&m_decoder, &decoder_options, NULL);
    base64_encoder_init(&m_encoder, &encoder_options, NULL);

    base64_encoder_write(&m_encoder, STR, strlen(STR));

    base64_encoder_finish(&m_encoder);
    base64_decoder_finish(&m_decoder);

    return 0;
}
