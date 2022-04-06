#ifndef MAPPING_DICT_H
#define MAPPING_DICT_H


#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "huffman_tree.h"


/**
 * @brief The mapping which maps one byte to its huffman codes.
 */
struct mapping_dict_mapping {
    uint8_t code[32];
    uint32_t bit_count;
};

/**
 * @brief The dictionary containing all the mappings of bytes to
 * their huffman codes.
 */
struct mapping_dict {
    struct mapping_dict_mapping mappings[256];
};

/**
 * @brief Creates an empty mapping dictionary.
 * 
 * @return struct mapping_dict* the created mapping dict.
 * Must be freed with a call to mapping_dict_free().
 */
struct mapping_dict* mapping_dict_create();

/**
 * @brief Creates a mapping dictionary from a huffman tree.
 * 
 * @param tree the huffman tree from which this mapping should be created.
 * @return struct mapping_dict* the created mapping dict.
 */
struct mapping_dict* mapping_dict_create_mapping(struct huffman_tree* tree);

/**
 * @brief Frees a mapping dict.
 * 
 * @param mapping_dict the mapping dict to be freed.
 */
void mapping_dict_free(struct mapping_dict* mapping_dict);


/**
 * @brief Prints the contents of a mapping dict to stdout.
 * 
 * @param mapping_dict the mapping dict which is supposed to be printed.
 */
void mapping_dict_print(struct mapping_dict* mapping_dict);

/**
 * @brief Returnd the mapping for a specified byte.
 * 
 * @param mapping_dict the dict that should be searched.
 * @param c the byte of which the mapping should be returned.
 * @return struct mapping_dict_mapping* the mapping for <c>.
 */
struct mapping_dict_mapping* mapping_dict_mapping_for(
    struct mapping_dict* mapping_dict, uint8_t c);


/**
 * @brief Returns the bit (0 or 1) at the given index.
 * 
 * @param mapping the mapping for fron which the bit should be retrieved.
 * @param index the index of the bit.
 * @return uint8_t the state of the bit (0 or 1).
 */
uint8_t mapping_dict_get_bit(struct mapping_dict_mapping* mapping,
    uint8_t index);

/**
 * @brief Sets the bit to 1 at a given index.
 * 
 * @param mapping the mapping for which the bit should be set.
 * @param index the index at which to set the bit.
 */
void mapping_dict_set_bit(struct mapping_dict_mapping* mapping,
    uint32_t index);


/**
 * @brief Compresses a file according to the codes in a mapping dict.
 * 
 * @param mapping_dict the mapping dict containg the byte => code mappings.
 * @param in_stream the stream of the file that should be compressed.
 * @param out_stream the stream to which the compressed contents
 * should be written.
 * @return int non-zero if an error occurred, zero otherwise.
 */
int mapping_dict_compress_file(struct mapping_dict* mapping_dict,
    FILE* in_stream, FILE* out_stream);


#endif
