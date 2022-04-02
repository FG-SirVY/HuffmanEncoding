#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "error.h"
#include "frequency_dict.h"
#include "linked_list.h"


/**
 * @brief The huffman tree, which is recurrent tree structure.
 */
struct huffman_tree {
    struct huffman_tree* left;
    struct huffman_tree* right;

    int number;
    int symbol;
    int frequency;
};


/**
 * @brief Creates a huffman tree as a leaf with the given symbol.
 * 
 * @param symbol the symbol this leaf should represent.
 * @return struct huffman_tree* the huffman tree with the specified <symbol>.
 * Must be freed with a call to huffman_tree_free().
 */
struct huffman_tree* huffman_tree_create_with_symbol(int symbol);

/**
 * @brief Creates an empty huffman tree.
 * 
 * @return struct huffman_tree* the huffman tree with the specified <symbol>.
 * Must be freed with a call to huffman_tree_free().
 */
struct huffman_tree* huffman_tree_create();

/**
 * @brief Creates a full huffman tree from a given file.
 * 
 * @param stream the stream from which the huffman tree should be created.
 * @return struct huffman_tree* the filled huffman tree.
 * Must be freed with a call to huffman_tree_free().
 */
struct huffman_tree* huffman_tree_create_from_stream(FILE* stream);

/**
 * @brief Reads huffman subtrees from a buffer.
 * 
 * @param parent the parent subtree of which the children should be populated.
 * @param buffer the buffer from which to read the child trees.
 * @return int non-zero when an error occurred, zero otherwise.
 */
int _huffman_tree_read_from_buffer(struct huffman_tree* parent, uint8_t* buffer);
/**
 * @brief Reads huffman trees from a stream.
 * 
 * @param stream the stream from which the tree should be created.
 * @return struct huffman_tree* the filled huffman tree.
 * Must be freed with a call to huffman_tree_free().
 */
struct huffman_tree* huffman_tree_read_from_stream(FILE* stream);

/**
 * @brief Frees a huffman_tree object.
 * 
 * @param tree the huffman tree to be freed.
 */
void huffman_tree_free(struct huffman_tree* tree);

/**
 * @brief Prints a huffman tree to stdout.
 * 
 * @param tree the tree to be printed.
 * @param indent the number of tabs this tree should be indented by.
 */
void huffman_tree_print(struct huffman_tree* tree, int indent);


/**
 * @brief Writes a huffman tree to a given buffer.
 * 
 * @param tree the huffman tree that should be written.
 * @param buffer the buffer that <tree> should be written to.
 * @return uint8_t* a pointer to the next free space in the buffer.
 */
uint8_t* _huffman_tree_write_to_buffer(struct huffman_tree* tree, uint8_t* buffer);

/**
 * 
 * @brief Writes a huffman tree to a stream.
 * 
 * @param tree the huffman tree that should be written.
 * @param stream the stream to which <tree> should be written.
 * @return int non-zero if an error occurred, zero otherwise.
 */
int huffman_tree_write_to_stream(struct huffman_tree* tree, FILE* stream);


/**
 * @brief Decompresses a file from a stream using the specified tree.
 * 
 * @param tree the huffman tree that should be used to decompress.
 * @param in_stream the stream that should be compressed.
 * @param out_stream the stream to which the compressed output should
 * be written.
 * @return int non-zero if an error occurred, zero otherwise.
 */
int huffman_tree_decompress_file(struct huffman_tree* tree, FILE* in_stream, FILE* out_stream);


#endif
