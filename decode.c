#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "types.h"
#include "common.h"


Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{

   if(argv[2] == NULL)
   {
    return e_failure;
   }

   if((strstr(argv[2], ".bmp")) != NULL)
    {
          decInfo -> stego_image_fname = argv[2];
    }

    else
    {
        return e_failure;
    }

    if(argv[3] != NULL)
    {
        decInfo -> output_secret_fname = argv[3];
    }

    else
    {
        decInfo -> output_secret_fname = "output.txt";
    }

    return e_success; 
}


Status open_filesd(DecodeInfo *decInfo)
{
    decInfo-> fptr_stego_image = fopen(decInfo-> stego_image_fname, "rb");
    // Do Error handling
    if (decInfo-> fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo-> stego_image_fname);

    	return e_failure;
    }

    // Secret file
    decInfo-> fptr_secret_file = fopen(decInfo ->output_secret_fname, "wb");
    // Do Error handling
    if (decInfo-> fptr_secret_file == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo ->output_secret_fname);

    	return e_failure;
    }

    return e_success;
}


char decode_byte_from_lsb(char *data_buffer)
{
    char ch = 0;
    for(int i=0; i<8; i++)
    {
       ch = ch | ((data_buffer[i] & 1) << i);
    }
   return ch;
}

Status decode_image_to_data(char *magic_string, int size, DecodeInfo *decInfo)
{
    char decoded_magic_string[size+1];
 
    for(int i=0; i<size; i++)
    {
        fread(decInfo -> stego_image_data, 8, 1, decInfo -> fptr_stego_image);
       decoded_magic_string[i] = decode_byte_from_lsb(decInfo -> stego_image_data);
    }

       decoded_magic_string[size] = '\0';

       if(strcmp(decoded_magic_string, magic_string)==0)
       {
          return e_success;
       }

       else
       {
          return e_failure;
       }
    
}

Status decode_size_from_lsb(DecodeInfo *decInfo)
{
    char byte;
    decInfo -> extn_size = 0;

    for(int i=0; i<32; i++)
    {
        fread(&byte, 1, 1, decInfo -> fptr_stego_image);
        decInfo -> extn_size = decInfo -> extn_size | ((byte & 1) << i);
    }
     return e_success;
     
}

Status decode_secret_file_size_from_lsb(DecodeInfo *decInfo)
{
    char byte;
    decInfo -> secret_file_size = 0;

    for(int i=0; i<32; i++)
    {
        fread(&byte, 1, 1, decInfo -> fptr_stego_image);
        decInfo -> secret_file_size =  decInfo -> secret_file_size | ((byte & 1) <<i);
    }
   
     return e_success;
     
}


Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
      return decode_image_to_data(magic_string, strlen(magic_string), decInfo);
    //  return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    return decode_size_from_lsb(decInfo);
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    decInfo -> extn = malloc(decInfo -> extn_size+1);
    if (decInfo->extn == NULL)
    return e_failure;


    for(int i=0; i<decInfo -> extn_size; i++)
    {
        fread(decInfo -> stego_image_data, 8, 1, decInfo -> fptr_stego_image);
        decInfo -> extn[i] = decode_byte_from_lsb(decInfo -> stego_image_data);
    }
    
    decInfo -> extn[decInfo -> extn_size] = '\0';
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    return decode_secret_file_size_from_lsb(decInfo);
    //return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;
    for(int i=0; i<decInfo -> secret_file_size; i++)
    {
        fread(decInfo -> stego_image_data, 8, 1, decInfo -> fptr_stego_image);
        ch = decode_byte_from_lsb(decInfo -> stego_image_data);
        fwrite(&ch, 1, 1, decInfo -> fptr_secret_file);
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
   if(open_filesd(decInfo) == e_success)
   {
        printf("Files are opened successfully\n");

        fseek(decInfo -> fptr_stego_image, 54, SEEK_SET);

        if(decode_magic_string(MAGIC_STRING, decInfo)== e_success)
        {
            printf("Magic strings found\nDecoded magic string successfully\n");
            if(decode_secret_file_extn_size(decInfo) == e_success)
            {
                printf("Decoded secret file extension size successfully\n");
                if(decode_secret_file_extn(decInfo) == e_success)
                {
                    printf("Decoded secret file extn successfully\n");
                    if(decode_secret_file_size(decInfo) == e_success)
                    {
                        printf("Decoded secret file size successfully\n");
                        if(decode_secret_file_data(decInfo) == e_success)
                        {
                            printf("Decoded secret file data successfully\n");
                            return e_success;
                        }
                        else
                        {
                            printf("Failed to decode secret file data\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("Failed to decode secret file size\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("failed to decode secret file extn\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Faileed to decode extn size\n");
                return e_failure;
            }
        }
        else
        {
            printf("Failed to decode magic string\n");
            return e_failure;
        }

    }
    else
    {
        printf("open files is failure\n");
        return e_failure;
    }
}