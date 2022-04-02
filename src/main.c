#include <stdio.h>
#include <string.h>
#include <time.h>

#include "error.h"
#include "linked_list.h"
#include "frequency_dict.h"
#include "mapping_dict.h"
#include "huffman_tree.h"


#define FILE_EXTENSION_COMPRESS ".huf"
#define FILE_EXTENSION_DECOMPRESS ".orig"


int compress_file(char* in_file_name) {
    char* out_file_name = malloc(strlen(in_file_name)
        + strlen(FILE_EXTENSION_COMPRESS) + 1);
    if (!out_file_name) {
        errno = ERR_MEM_ERROR;
        return 1;
    }

    strcpy(out_file_name, in_file_name);
    strcat(out_file_name, FILE_EXTENSION_COMPRESS);

    clock_t start, end;
    start = clock();

    FILE* in_stream = fopen(in_file_name, "rb");
    if (!in_stream) {
        free(out_file_name);
        return 1;
    }
    FILE* out_stream = fopen(out_file_name, "wb");
    if (!out_stream) {
        fclose(in_stream);
        free(out_file_name);
        return 1;
    }

    struct huffman_tree* tree = huffman_tree_create_from_stream(in_stream);
    if (!tree) {
        fclose(in_stream);
        free(out_file_name);
        return 1;
    }
    struct mapping_dict* mapping_dict = mapping_dict_create_mapping(tree);
    if (!mapping_dict) {
        huffman_tree_free(tree);
        fclose(in_stream);
        free(out_file_name);
        return 1;
    }

    int error_code = (fseek(in_stream, 0, 0)
        || huffman_tree_write_to_stream(tree, out_stream)
        || mapping_dict_compress_file(mapping_dict, in_stream, out_stream));

    mapping_dict_free(mapping_dict);
    huffman_tree_free(tree);
    fclose(out_stream);
    fclose(in_stream);

    end = clock();
    double time_used = ((double) end - start) / CLOCKS_PER_SEC;

    if (!error_code) {
        printf("Compressed file %s to %s in %fs.\n",
            in_file_name, out_file_name, time_used);
        free(out_file_name);
        return 0;
    } else {
        free(out_file_name);
        return 1;
    }
}


int decompress_file(char* in_file_name) {
    char* out_file_name = malloc(strlen(in_file_name)
        + strlen(FILE_EXTENSION_DECOMPRESS) + 1);
    if (!out_file_name) return 1;

    strcpy(out_file_name, in_file_name);
    strcat(out_file_name, FILE_EXTENSION_DECOMPRESS);

    clock_t start, end;
    start = clock();

    FILE* in_stream = fopen(in_file_name, "rb");
    if (!in_stream) {
        free(out_file_name);
        return 1;
    }
    FILE* out_stream = fopen(out_file_name, "wb");
    if (!out_stream) {
        fclose(in_stream);
        free(out_file_name);
        return 1;
    }

    struct huffman_tree* tree = huffman_tree_read_from_stream(in_stream);
    if (!tree) {
        fclose(in_stream);
        free(out_file_name);
        return 1;
    }

    int error_code = huffman_tree_decompress_file(tree, in_stream, out_stream);

    huffman_tree_free(tree);
    fclose(out_stream);
    fclose(in_stream);

    end = clock();
    double time_used = ((double) end - start) / CLOCKS_PER_SEC;

    if (!error_code) {
        printf("Deompressed file %s to %s in %fs.\n", in_file_name, out_file_name, time_used);
        free(out_file_name);
        return 0;
    } else {
        free(out_file_name);
        return 1;
    }
}


int main(int argc, char* argv[]) {
    printf("Enter c to compress, d to decompress, x to exit:\n");
    char c = getchar();

    switch (c) {
        case 'c': {
            printf("Enter the path to the file which should be compressed:\n");
            char buffer[256];
            scanf("%255s", buffer);
            if (compress_file(buffer)) {
                print_error("Failed to compress file");
            }
            break;
        }
        case 'd': {
            printf("Enter the path to the file which should be decompressed:\n");
            char buffer[256];
            scanf("%255s", buffer);
            if (decompress_file(buffer)) {
                print_error("Failed to compress file");
            }
            break;
        }
        case 'x':
            return 0;
    }
}
