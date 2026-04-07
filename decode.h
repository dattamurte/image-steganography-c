#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_DATA_SIZE  (MAX_SECRET_BUF_SIZE * 8)

typedef struct _DecodeInfo
{
    char *stego_image_fname;
    FILE *fptr_stego_image;
    char stego_image_data[MAX_IMAGE_DATA_SIZE];

    char *output_secret_fname;
    FILE *fptr_secret_file;

    int extn_size;
    int secret_file_size;
    char *extn;

}DecodeInfo;


Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

Status open_filesd(DecodeInfo *decInfo);

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

Status decode_image_to_data(char *magic_string, int size, DecodeInfo *decInfo);

char decode_byte_from_lsb(char *image_buffer);

Status decode_secret_file_extn_size(DecodeInfo *decInfo);

Status decode_size_from_lsb(DecodeInfo *decInfo);

Status decode_secret_file_extn(DecodeInfo *decInfo);

Status decode_secret_file_size(DecodeInfo *decInfo);

Status decode_secret_file_size_from_lsb(DecodeInfo *decInfo);

Status decode_secret_file_data(DecodeInfo *decInfo);

#endif
