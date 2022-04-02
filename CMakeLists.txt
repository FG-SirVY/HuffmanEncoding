cmake_minimum_required(VERSION 3.12)
project(HuffmanEncoding)
add_executable(encoder src/main.c src/linked_list.c
    src/frequency_dict.c src/huffman_tree.c
    src/mapping_dict.c src/error.c)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
