#include "error.h"


void print_error(const char* msg) {
    if (errno < 200) {
        perror(msg);
    } else {
        char *error_description;

        switch (errno) {
            case ERR_ILLEGAL_ARG:
                error_description = ERR_ILLEGAL_ARG_STR;
                break;
            case ERR_MEM_ERROR:
                error_description = ERR_MEM_ERROR_MSG;
                break;
            case ERR_IO_ERROR:
                error_description = ERR_IO_ERROR_MSG;
                break;
            case ERR_PARSE_ERROR:
                error_description = ERR_PARSE_ERROR_MSG;
                break;
        }

        fprintf(stderr, "%s:%s\n", msg, error_description);
    }
}
