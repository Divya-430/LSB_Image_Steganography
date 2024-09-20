#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

typedef struct _DecodeInfo
{
    /* output file info */
    char stego_image_fname[100];  /* to store the file name (output) */
    FILE *fptr_stego_image;

    /* secret file info */
    char secret_file_fname[100];
    FILE *fptr_secret_file;

    /*decoded magic string info*/
    char decoded_magic_string[100];
    uint decoded_magic_str_size;

    /* secret file extension info */
    uint decoded_file_extn_size;
    char decoded_file_extn[20];

    /* output secret file info*/
    uint decoded_secret_msg_length;
    char decoded_secret_msg[1000000];
    
} DecodeInfo;

/*Read and validate decode args from argv  */
Status1 read_and_validate_decode_args(char *argv[], DecodeInfo *deccInfo);

/* for decoding operation */
Status1 do_decoding(DecodeInfo *decInfo);

/* for decoding magic string length */
Status1 decode_magic_string_length(DecodeInfo *decInfo);

/* for decoding an integer */
int decode_integer(char *image_buffer,DecodeInfo *decInfo);

/* for decoding magic string */
Status1 decode_magic_string(DecodeInfo *decInfo);

/* for decoding a string */
 char *decode_string(int size, char *str, DecodeInfo *decInfo);

/* for decoding a secret file extension length*/
Status1 decode_file_extn_length(DecodeInfo *decInfo);

/* for decoding a secret file extension */
Status1 decode_file_extn(long file_extension_length,DecodeInfo *decInfo);

/* for decoding a secret msg length */
Status1 decode_secret_msg_length(DecodeInfo *decInfo);

/* for decoding a secret msg */
Status1 decode_secret_msg(long file_extension_length, DecodeInfo *decInfo);

#endif