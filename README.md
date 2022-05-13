# HuffmanEncoding

# Motivation
Creating a fast native version of a introductory university project in computer science.
The original implementation was very constrained and written in Python. It was VERY slow,
to say the least.


# Overview
The application can encode and decode any kind of file using Huffman encoding. First,
an optimal HuffmanTree is generated from a frequency analysis of all the bytes in the
file that should be encoded. Then, every byte is encoded according to the bit sequence
obtained from that Huffman Tree. The tree is stored at the beginning of the encoded file,
followed by the encoded file content.


# Requirements
- CMake ^3.12
- A suitable C compiler


# Usage
Run the compiled executable (build/Release), the prompts presented should be clear.
