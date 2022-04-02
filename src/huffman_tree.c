#include "huffman_tree.h"


#define BUFFER_SIZE 65536


struct huffman_tree* huffman_tree_create_with_symbol(int symbol) {
    struct huffman_tree* ret = calloc(1, sizeof(struct huffman_tree));
    if (ret) ret->symbol = symbol;
    else errno = ERR_MEM_ERROR;

    return ret;
}

struct huffman_tree* huffman_tree_create() {
    return huffman_tree_create_with_symbol(-1);
}

void huffman_tree_free(struct huffman_tree* tree) {
    if (tree->left) huffman_tree_free(tree->left);
    if (tree->right) huffman_tree_free(tree->right);

    free(tree);
}

void huffman_tree_print(struct huffman_tree* tree, int indent) {
    for (int i = 0; i < indent; i++) {
        putchar('\t');
    }

    if (tree->symbol >= 0) {
        printf("%d|%c\n", tree->frequency, tree->symbol);
    } else {
        printf("%d\n", tree->frequency);
    }

    if (tree->right) huffman_tree_print(tree->right, indent + 1);
    if (tree->left) huffman_tree_print(tree->left, indent + 1);
}

int huffman_tree_find_tree_after(struct huffman_tree* tree_after,
        struct huffman_tree* tree) {
    if (tree_after->frequency == tree->frequency)
        return tree->symbol >= 0 && tree_after->symbol < 0;
    else
        return tree_after->frequency > tree->frequency;
}

int huffman_tree_number_nodes(struct huffman_tree* tree, int number) {
    if (tree && tree->symbol < 0) {
        number = huffman_tree_number_nodes(tree->left, number);
        number = huffman_tree_number_nodes(tree->right, number);

        tree->number = number;
        number += 1;
    }

    return number;
}

struct huffman_tree* huffman_tree_create_from_stream(FILE* stream) {
    struct freq_dict* dict = freq_dict_create_from_stream(stream);
    if (!dict) return NULL;

    struct linked_list* list = linked_list_create();
    if (!list) {
        freq_dict_free(dict);
        return NULL;
    }

    for (int i = 0; i < 256; i++) {
        int frequency = freq_dict_frequency_for(dict, i);
        if (!frequency) continue;

        struct huffman_tree* tree = huffman_tree_create_with_symbol(i);
        tree->frequency = frequency;

        struct linked_list_node* before_node
            = linked_list_find_node(list, huffman_tree_find_tree_after, tree);
        linked_list_insert_before(list, before_node, tree);
    }

    freq_dict_free(dict);

    while (list->length > 1) {
        struct huffman_tree* left = linked_list_pop(list, 0);
        struct huffman_tree* right = linked_list_pop(list, 0);

        struct huffman_tree* tree = huffman_tree_create();
        tree->frequency = left->frequency + right->frequency;
        tree->left = left;
        tree->right = right;

        struct linked_list_node* before_node
            = linked_list_find_node(list, huffman_tree_find_tree_after, tree);
        linked_list_insert_before(list, before_node, tree);
    }

    struct huffman_tree* ret = linked_list_pop(list, 0);
    linked_list_free(list);

    huffman_tree_number_nodes(ret, 0);

    return ret;
}

uint8_t* _huffman_tree_write_to_buffer(struct huffman_tree* tree,
        uint8_t* buffer) {
    uint8_t local_buffer[4];

    if (tree->left->symbol >= 0) {
        local_buffer[0] = 0;
        local_buffer[1] = tree->left->symbol;
    } else {
        local_buffer[0] = 1;
        local_buffer[1] = tree->left->number;
        buffer = _huffman_tree_write_to_buffer(tree->left, buffer);
    }

    if (tree->right->symbol >= 0) {
        local_buffer[2] = 0;
        local_buffer[3] = tree->right->symbol;
    } else {
        local_buffer[2] = 1;
        local_buffer[3] = tree->right->number;
        buffer = _huffman_tree_write_to_buffer(tree->right, buffer);
    }

    memcpy(buffer, local_buffer, 4);

    return buffer + 4;
}

int huffman_tree_write_to_stream(struct huffman_tree* tree, FILE* stream) {
    uint8_t num_nodes = tree->number + 1;
    size_t buffer_size = (size_t)num_nodes * 4 + 1;

    char* buffer = malloc(buffer_size);
    if (!buffer) {
        errno = ERR_MEM_ERROR;
        return 1;
    }

    buffer[0] = num_nodes;
    
    _huffman_tree_write_to_buffer(tree, buffer + 1);

    int bytes_written = (int)fwrite(buffer, 1, buffer_size, stream);
    free(buffer);

    return bytes_written != buffer_size;
}

int _huffman_tree_read_from_buffer(struct huffman_tree* parent,
        uint8_t* buffer) {
    uint8_t* read_node = buffer + 4 * parent->number;

    if (read_node[0] == 0) {
        parent->left = huffman_tree_create_with_symbol(read_node[1]);
        if (!parent->left) return 1;
    } else {
        parent->left = huffman_tree_create();
        if (!parent->left) return 1;
        parent->left->number = read_node[1];

        if (_huffman_tree_read_from_buffer(parent->left, buffer)) return 1;
    }
    if (read_node[2] == 0) {
        parent->right = huffman_tree_create_with_symbol(read_node[3]);
        if (!parent->right) return 1;
    } else {
        parent->right = huffman_tree_create();
        if (!parent->right) return 1;
        parent->right->number = read_node[3];

        if (_huffman_tree_read_from_buffer(parent->right, buffer)) return 1;
    }

    return 0;
}

struct huffman_tree* huffman_tree_read_from_stream(FILE* stream) {
    uint8_t num_nodes = 0;
    if (fread(&num_nodes, 1, 1, stream) != 1) return NULL;

    size_t buffer_size = (size_t)num_nodes * 4;

    uint8_t* buffer = malloc(buffer_size);
    if (!buffer) {
        errno = ERR_MEM_ERROR;
        return NULL;
    }

    if (fread(buffer, 1, buffer_size, stream) != buffer_size) {
        free(buffer);
        errno = ERR_PARSE_ERROR;
        return NULL;
    }

    struct huffman_tree* root = huffman_tree_create();
    root->number = num_nodes - 1;
    if (!root) {
        free(buffer);
        return NULL;
    }
    int result = _huffman_tree_read_from_buffer(root, buffer);
    free(buffer);

    if (result) return NULL;
    else return root;
}

int huffman_tree_decompress_file(struct huffman_tree* tree,
        FILE* in_stream, FILE* out_stream) {
    uint8_t* in_buffer = malloc(2 * BUFFER_SIZE);
    uint8_t* out_buffer = in_buffer + BUFFER_SIZE;
    if (!in_buffer) {
        errno = ERR_MEM_ERROR;
        return 1;
    }

    uint32_t num_bytes = 0;
    if (fread(&num_bytes, sizeof(uint32_t), 1, in_stream) != 1) {
        return ERR_PARSE_ERROR;
        return 1;
    }

    int bytes_converted = 0;
    int write_byte_index = 0; 
    struct huffman_tree* current_tree = tree;

    while (1) {
        size_t read = fread(in_buffer, 1, BUFFER_SIZE, in_stream);

        for (size_t i = 0; i < read; i++) {
            uint8_t current_byte = in_buffer[i];

            for (int j = 7; j >= 0; j--) {
                if ((current_byte >> j) & 1)
                    current_tree = current_tree->right;
                else
                    current_tree = current_tree->left;

                if (current_tree->symbol >= 0) {
                    out_buffer[write_byte_index] = current_tree->symbol;
                    current_tree = tree;
                    bytes_converted++;
                    write_byte_index++;

                    if (write_byte_index == BUFFER_SIZE
                            || bytes_converted == num_bytes) {
                        if (fwrite(out_buffer, 1, write_byte_index, out_stream)
                                != write_byte_index) {
                            errno = ERR_IO_ERROR;
                            return 1;
                        }
                        
                        write_byte_index = 0;

                        if (bytes_converted == num_bytes) {
                            free(in_buffer);
                            return 0;
                        }
                    }
                }
            }
        }
    }
}
