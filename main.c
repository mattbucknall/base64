#include <stdio.h>
#include <string.h>

#include <base64.h>

int main()
{
    static const base64_options_t options =
    {
        .with_lines = 1,
        .null_terminate = 1
    };

    const char* in = "Here is a load of text to base64 encode, to see if things are working.";
    size_t in_len = strlen(in);
    char out[1024];
    size_t out_len = base64_buffer_size_from_raw_buffer_size(in_len, &options);

    base64_from_raw(out, out_len, in, in_len, &options);

    printf("%lu %lu %lu\n", in_len, out_len, strlen(out));
    printf("'%s'\n", out);

    base64_buffer_size_to_raw_buffer_size(0);

    return 0;
}
