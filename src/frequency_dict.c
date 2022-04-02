#include "frequency_dict.h"


#define BUFFER_SIZE 32768


struct freq_dict* freq_dict_create() {
    struct freq_dict* frequency_dict = malloc(sizeof(struct freq_dict));
    if (!frequency_dict) {
        errno = ERR_MEM_ERROR;
        return NULL;
    }

    frequency_dict->frequencies = calloc(256, sizeof(int));
    if (!frequency_dict->frequencies) {
        free(frequency_dict);
        errno = ERR_MEM_ERROR;
        return NULL;
    }

    return frequency_dict;
}

void freq_dict_free(struct freq_dict* frequency_dict) {
    free(frequency_dict->frequencies);
    free(frequency_dict);
}


int freq_dict_frequency_for(struct freq_dict* frequency_dict, uint8_t c) {
    return frequency_dict->frequencies[c];
}


void freq_dict_print(struct freq_dict* frequency_dict) {
    for (int i = 0; i < 256; i++) {
        int frequency = freq_dict_frequency_for(frequency_dict, i);
        if (frequency > 0) {
            printf("%c:\t%d\n", i, frequency);
        }
    }
}


struct freq_dict* freq_dict_create_from_stream(FILE* stream) {
    struct freq_dict* ret = freq_dict_create();
    if (!ret) {
        errno = ERR_MEM_ERROR;
        return NULL;
    }

    uint8_t *buffer = malloc(BUFFER_SIZE);
    if (!buffer) {
        errno = ERR_MEM_ERROR;
        freq_dict_free(ret);
        return NULL;
    }
    size_t read = 0;
    do {
        read = fread(buffer, sizeof(uint8_t), BUFFER_SIZE, stream);
        for (size_t i = 0; i < read; i++) {
            ret->frequencies[buffer[i]] += 1;
        }
    } while (read != 0);
    free(buffer);

    if (fseek(stream, 0, 0)) {
        freq_dict_free(ret);
        errno = ERR_IO_ERROR;
        return NULL;
    }

    return ret;
}
