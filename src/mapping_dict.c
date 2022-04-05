#include "mapping_dict.h"


#define BUFFER_SIZE 65536


struct mapping_dict* mapping_dict_create() {
    struct mapping_dict* ret = calloc(1, sizeof(struct mapping_dict));
    if (!ret) errno = ERR_MEM_ERROR;

    return ret;
}

void mapping_dict_free(struct mapping_dict* mapping_dict) {
    free(mapping_dict);
}

void mapping_dict_print(struct mapping_dict* mapping_dict) {
    for (int i = 0; i < 256; i++) {
        struct mapping_dict_mapping* mapping
            = mapping_dict_mapping_for(mapping_dict, i);

        if (mapping->bit_count > 0) {
            for (uint32_t j = 0; j < mapping->bit_count; j++) {
                putchar('0' + get_bit(mapping_dict->mappings + i, j));
            }
            printf("->%d (%d)\n", i, mapping->bit_count);
        }
    }
}

struct mapping_dict_mapping* mapping_dict_mapping_for(
        struct mapping_dict* mapping_dict, uint8_t c) {
    return mapping_dict->mappings + c;
}

uint8_t get_bit(struct mapping_dict_mapping* mapping, uint8_t index) {
    int byte = index / 8;
    int bit = index - 8 * byte;

    return (uint8_t)1 & (mapping->code[byte] >> (7 - bit));
}

void set_bit(struct mapping_dict_mapping* mapping, uint32_t index) {
    int byte = index / 8;
    int bit = index - 8 * byte;

    mapping->code[byte] |= 1 << (7 - bit);
}

void _md_create_mapping(struct huffman_tree* tree,
        struct mapping_dict* mapping_dict, struct mapping_dict_mapping* prev) {
    if (tree->symbol >= 0) {
        memcpy(&mapping_dict->mappings[tree->symbol], prev,
            sizeof(struct mapping_dict_mapping));
    } else {
        struct mapping_dict_mapping next;

        memcpy(&next, prev, sizeof(struct mapping_dict_mapping));
        next.bit_count++;
        _md_create_mapping(tree->left, mapping_dict, &next);

        set_bit(&next, next.bit_count - 1);
        _md_create_mapping(tree->right, mapping_dict, &next);
    }
}

struct mapping_dict* mapping_dict_create_mapping(struct huffman_tree* tree) {
    struct mapping_dict* mapping_dict = mapping_dict_create();
    if (!mapping_dict) return NULL;

    struct mapping_dict_mapping next;
    memset(&next, 0, sizeof(struct mapping_dict_mapping));
    _md_create_mapping(tree, mapping_dict, &next);

    return mapping_dict;
}

int mapping_dict_compress_file(struct mapping_dict* mapping_dict,
        FILE* in_stream, FILE* out_stream) {
    uint8_t* in_buffer = malloc(2 * BUFFER_SIZE);
    uint8_t* out_buffer = in_buffer + BUFFER_SIZE;
    if (!in_buffer) {
        errno = ERR_MEM_ERROR;
        return 1;
    }

    int length;
    int error_code = (fseek(in_stream, 0L, SEEK_END)
        || !(length = (int)ftell(in_stream))
        || fseek(in_stream, 0, 0));

    if (error_code) {
        errno = ERR_IO_ERROR;
        free(in_buffer);
        return 1;
    }

    (*(int*)out_buffer) = length;

    size_t read = 0;
    size_t write_index = 4;
    uint8_t current_byte = 0;
    int bit_index = 7;

    do {
        read = fread(in_buffer, sizeof(uint8_t), BUFFER_SIZE, in_stream);
        for (size_t i = 0; i < read; i++) {
            struct mapping_dict_mapping* current_mapping
                = mapping_dict->mappings + in_buffer[i];
            for (uint32_t j = 0; j < current_mapping->bit_count; j++) {
                current_byte |= get_bit(current_mapping, j) << bit_index;
                bit_index -= 1;

                if (bit_index < 0) {
                    out_buffer[write_index] = current_byte;
                    current_byte = 0;
                    bit_index = 7;
                    write_index += 1;

                    if (write_index == BUFFER_SIZE) {
                        if (fwrite(out_buffer, sizeof(uint8_t),
                                BUFFER_SIZE, out_stream) != BUFFER_SIZE) {
                            free(in_buffer);
                            errno = ERR_IO_ERROR;
                            return 1;
                        }
                        write_index = 0;
                    }
                }
            }
        }

    } while(read != 0);

    if (bit_index < 7) {
        out_buffer[write_index] = current_byte;
        write_index += 1;
    }

    if (write_index > 0 && fwrite(out_buffer, 1, write_index, out_stream)
            != write_index) {
        errno = ERR_IO_ERROR;
        free(in_buffer);
        return 1;
    }

    free(in_buffer);
    return 0;
}
