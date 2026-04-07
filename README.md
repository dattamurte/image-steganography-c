# Image Steganography in C

## Description
This project implements image steganography using C programming to hide and extract secret data inside BMP image files.

## Features
- Encode secret data into BMP image
- Decode hidden data from image
- Supports text file hiding
- Uses Least Significant Bit (LSB) method
- Works with BMP image format

## Technologies Used
- C Programming
- File Handling
- Bitwise Operations
- Structures
- BMP Image Processing

## Files Included
- encode.c
- decode.c
- common.h
- types.h
- test_encode.c

## How to Compile
make

## How to Run

Encoding:
./a.out -e beautiful.bmp secret.txt stego.bmp

Decoding:
./a.out -d stego.bmp output.txt