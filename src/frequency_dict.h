#ifndef FREQUENCY_DICT_H
#define FREQUENCY_DICT_H


#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "error.h"


/**
 * @brief The frequency dict holding all the information on the
 * occurence of characters.
 */
struct freq_dict {
    int* frequencies;
};

/**
 * @brief Creates a frequency dictionary on the heap.
 * 
 * @return struct freq_dict* the created frequency dict.
 * Must be freed by freq_dict_free().
 */
struct freq_dict* freq_dict_create();

/**
 * @brief Frees a frequency dict object.
 * 
 * @param frequency_dict 
 */
void freq_dict_free(struct freq_dict* frequency_dict);


/**
 * @brief Returns the frequency of occurence of a given byte.
 * 
 * @param frequency_dict the frequency dict for lookup.
 * @param c the byte of which the frequency should be returned.
 * @return int the frequency with which <c> occurs.
 */
int freq_dict_frequency_for(struct freq_dict* frequency_dict, uint8_t c);

/**
 * @brief Prints the contents of the frequency dict to stdout.
 * 
 * @param frequency_dict the frequency dict that should be printed.
 */
void freq_dict_print(struct freq_dict* frequency_dict);

/**
 * @brief Creates a frequency dict from a file stream.
 * 
 * @param stream the stream that should be analyzed.
 * @return struct freq_dict* the frequency dict with
 * all the frequencies of characters occurring in <stream>.
 * Must be freed by freq_dict_free().
 */
struct freq_dict* freq_dict_create_from_stream(FILE* stream);


#endif
