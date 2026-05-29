#include "bit_buffer.h"

void init_bw(struct bit_writer *writer, const struct ubuffer *bits)
{
    writer->bit_len = bits->len;
    writer->byte_len = 0;
    writer->bits = malloc(bits->len + 1);
    writer->bits[bits->len] = '\0';
    copy_ucstr(writer->bits, bits->buffer, bits->len);
}

void free_bw(struct bit_writer *writer)
{
    free(writer->bits);
}

void convert_table(struct ubuffer *out_buffer, const struct ubuffer *table)
{
    struct ubuffer result;
    unsigned long i, size;
    unsigned char ch[1];

    init_ubuf(&result, u(""), 0);

    size = 0;
    for (i = 0; i < ASCII_SIZE; i++)
        if (table[i].buffer != NULL)
            size++;

    add_long_ubuf(&result, size);
    add_ubuf(&result, u("\n"), 1);

    for (i = 0; i < ASCII_SIZE; i++) {
        if (table[i].buffer != NULL) {
            ch[0] = (unsigned char) i;
#ifdef BEAUTIFUL_TABLE
            if (ch[0] != '\n') {
                add_ubuf(&result, u("'"), 1);
#endif
                add_ubuf(&result, ch, 1);
#ifdef BEAUTIFUL_TABLE
                add_ubuf(&result, u("'"), 1);
            } else {
                add_ubuf(&result, u("\\n"), 2);
            }
            add_ubuf(&result, u(" "), 1);

#endif
            add_ubuf(&result, u(" "), 1);
            
            add_ubuf(
                &result,
                table[i].buffer,
                table[i].len
            );
            add_ubuf(&result, u("\n"), 1);
        }
    }

#ifdef BEAUTIFUL_TABLE
    add_ubuf(&result, u("\n"), 1);
#endif
    move_ubuf(out_buffer, &result);
}

void deconvert_table(
        struct ubuffer *table,
        const struct ubuffer *in_buffer,
        unsigned long *cursor
) {
    unsigned long table_size, code_size, i, j;
    unsigned char ch;

    table_size = 0;
    for (i = 0; in_buffer->buffer[*cursor + i] != '\n'; i++)
        table_size = table_size * 10 + (in_buffer->buffer[*cursor + i] - '0');
    *cursor += i;

    j = 0;
    for (i = 0; i < table_size; i++) {
        ch = in_buffer->buffer[*cursor + i];
        for (j = 0; in_buffer->buffer[*cursor + i + j] != '\n'; j++) {
            add_ubuf(
                table + ch,
                in_buffer->buffer + *cursor + j,
                1);
        }
        *cursor += j;
    }
    *cursor += i;
}


void convert_bits(struct ubuffer *out_buffer, struct bit_writer *writer)
{
    unsigned char *result;
    unsigned int byte_cap;
    unsigned char byte;
    int i;
    
    byte_cap = (writer->bit_len + 7) / 8;
    result = malloc(byte_cap);

    byte = 0;
    for (i = 0; i < writer->bit_len; i++) {
        byte <<= 1;

        if (writer->bits[i] == '1')
            byte |= 1;

        if (i % 8 == 7) {
            result[writer->byte_len] = byte;
            writer->byte_len++;
            byte = 0;
        }
    }

    if (i % 8 != 0) {
        byte <<= 8 - (i % 8);
        result[writer->byte_len] = byte;
        writer->byte_len++;
    }

    add_ubuf(out_buffer, result, writer->byte_len);
    free(result);
}

void deconvert_bits(
        struct ubuffer *ubuf,
        const struct ubuffer *in_buffer,
        unsigned long *cursor
) {
    unsigned long byte_len, bit_len, i;

    byte_len = 0;
    for (i = 0; in_buffer->buffer[*cursor + i] != '\n'; i++)
        byte_len = byte_len * 10 + (in_buffer->buffer[i] - '0');
    *cursor += i;

    for (i = 0; i < byte_len; i++)
        add_ubuf(ubuf, in_buffer->buffer + *cursor + i, 1);
    *cursor += i;
}


