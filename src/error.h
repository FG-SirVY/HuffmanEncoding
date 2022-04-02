#ifndef ERROR_H
#define ERROR_H


#include <stdio.h>
#include <errno.h>

#define ERR_ILLEGAL_ARG 200
#define ERR_ILLEGAL_ARG_STR "Illegal argument specified"
#define ERR_MEM_ERROR 201
#define ERR_MEM_ERROR_MSG "Could not allocate required memory"
#define ERR_IO_ERROR 202
#define ERR_IO_ERROR_MSG "IO Error"
#define ERR_PARSE_ERROR 203
#define ERR_PARSE_ERROR_MSG "File format errors found"


/**
 * @brief Prints an error to stderr.
 * 
 * @param msg The message that is associated with it.
 */
void print_error(const char* msg);


#endif
