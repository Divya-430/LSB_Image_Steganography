/* Name: K. Divya
   Date: 26/08/2024
   Description: LSB Image Steganography Project
*/
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

/* function definition to read and validate decode args */
Status1 read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    /* checking if argv[2] is a .bmp file or not */
    char *str = strstr(argv[2], ".");
    if(str==NULL)
    {
        
        return d_failure;
    }
    if (strcmp(str,".bmp")==0)
    {
        strcpy(decInfo->stego_image_fname,argv[2]);
    }
    else
    {
        return d_failure;
    }
     /* checking if argv[2] is NULL*/
    if (argv[3] != NULL)
    {
            strcpy(decInfo->secret_file_fname,argv[3]);
    }
    else
    {
        printf("INFO: Output File not mentioned. Creating deafult_secret_file. as default \n");
        strcpy(decInfo->secret_file_fname,"default_secret_out");
    }
    return d_success;
}
/* function definition for decoding  */
Status1 do_decoding(DecodeInfo *decInfo)
{
    printf("## INFO: Decoding procedure started ##\n");
    printf("INFO: Opening required files \n");
     /* opening output file in read mode */
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    if(decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);;

        return e_failure;
    }
    printf("INFO: Opened %s \n", decInfo->stego_image_fname);
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    printf("INFO: Pointer Pointing to %ldth byte in stego.bmp file \n", ftell(decInfo->fptr_stego_image));

    /* decoding magic string length */
    printf("INFO: Decoding Magic string Size \n");
    int res1 = decode_magic_string_length(decInfo);
    if (res1 == d_success)
    {
        printf("INFO: Done \n");
    }
    else
    {
        printf("INFO: Magic string size is greater than 10 \n");
        printf("INFO: Decoding is not done \n");
        return d_failure;
    }

    /* decoding magic string*/
    printf("INFO: Decoding Magic string \n");
    int res2 = decode_magic_string(decInfo);
    if (res2 == d_success)
    {
        printf("INFO: Done \n");
    }

    /* reading another magic string from user */
    char magic_string2[20];
    printf("INFO: Enter a magic string to decode the Message \n");
    scanf("%[^\n]", magic_string2);
    getchar();
    if (strcmp(decInfo->decoded_magic_string, magic_string2) == 0)
    {
        printf("INFO: Entered magic string is correct \n");
        /* decoding secret file extension length */
        printf("INFO: Decoding Output File Extension Size \n");
        int res3 = decode_file_extn_length(decInfo);
        if (res3 == d_success)
        {
            printf("INFO: Done \n");
        }

        /* decoding secret file extension */
        printf("INFO: Decoding Output File Extension \n");
        int res4 = decode_file_extn(decInfo->decoded_file_extn_size, decInfo);
        if (res4 == d_success)
        {
            printf("INFO: Done \n");
        }

        /* merging file name and extension*/
        strcat(decInfo->secret_file_fname, decInfo->decoded_file_extn);
        printf("INFO: Output Secret File Name is %s\n", decInfo->secret_file_fname);
        decInfo->fptr_secret_file = fopen(decInfo->secret_file_fname, "w");

        /* decoding secret file length */
        printf("INFO: Decoding %s File Size \n", decInfo->secret_file_fname);
        int res5 = decode_secret_msg_length(decInfo);
        if(res5 == d_success)
        {
            printf("INFO: Done \n");
        }

        /* decoding secret msg*/
        printf("INFO: Decoding %s File Data \n", decInfo->secret_file_fname);
        int res6 = decode_secret_msg(decInfo->decoded_secret_msg_length, decInfo);
        if (res6 == d_success)
        {
            /* secret msg printing to output secret file */
            fprintf( decInfo->fptr_secret_file,"%s", decInfo->decoded_secret_msg);
            printf("INFO: Done \n");
        }
        printf("INFO: ## Decoding Done Successfully ## \n");
        fclose(decInfo->fptr_stego_image);
        fclose(decInfo->fptr_secret_file);
        
    }
    else
    {
        printf("INFO: Entered magic string is not matched \n");
    }
}
/* function definition for decoding magic string length (integer) */
Status1 decode_magic_string_length(DecodeInfo *decInfo)
{
    char temp_buffer[32];
    /* read 32 bytes from stego file and store into temp_buffer */
    fread(temp_buffer, 32, 1, decInfo->fptr_stego_image);
    /* function call to decode integer*/
    decInfo->decoded_magic_str_size = decode_integer(temp_buffer, decInfo);
    if(decInfo->decoded_magic_str_size>10)
    {
        return d_failure;
    }
    else
    {
        return d_success;
    }
}

/* function definition for decoding an integer data */
int decode_integer(char *image_buffer, DecodeInfo *decInfo)
{
    int length = 0;
    for (int i = 0; i < 32; i++)
    {
        int bit = image_buffer[i] & 1;
        length = length | (bit << i);
    }
    return length;
}
/* function definition for decoding magic string (string)  */
Status1 decode_magic_string(DecodeInfo *decInfo)
{
    char str[100];
    /* function call to decode string */
    char *ptr = decode_string(decInfo->decoded_magic_str_size, str, decInfo);
    strcpy(decInfo->decoded_magic_string, ptr);
    return d_success;
}
/* function definition for decoding a string */
char *decode_string(int size, char *str, DecodeInfo *decInfo)
{
    char buffer[8];
    char num;
    int bit2;
    for (int i = 0; i < size; i++)
    {
        num = 0;
        fread(buffer, 8, 1, decInfo->fptr_stego_image);
        for (int j = 0; j < 8; j++)
        {
            bit2 = buffer[j] & 1;
            num = num | bit2 << j;
        }
        str[i] = num;
    }
    str[size] = '\0';
    return str;
}

/* function definition for decoding secret file extension length (integer) */
Status1 decode_file_extn_length(DecodeInfo *decInfo)
{
    char temp_buffer[32];
    /* read 32 bytes from stego file and store into temp_buffer */
    fread(temp_buffer, 32, 1, decInfo->fptr_stego_image);
    /* function call to decode integer */
    decInfo->decoded_file_extn_size = decode_integer(temp_buffer, decInfo);
    return d_success;
}
/* function definition for decoding secret file extension (string)   */
Status1 decode_file_extn(long file_extension_length, DecodeInfo *decInfo)
{
    char str1[20];
    /* function call to decode string */
    char *p = decode_string(file_extension_length, str1, decInfo);
    strcpy(decInfo->decoded_file_extn, p);
    return d_success;
}
/* function definition for decoding secret msg length (integer) */
Status1 decode_secret_msg_length(DecodeInfo *decInfo)
{
    char temp_buffer[32];
    /* read 32 bytes from stego file and store into temp_buffer */
    fread(temp_buffer, 32, 1, decInfo->fptr_stego_image);
    /* function call to decode integer */
    decInfo->decoded_secret_msg_length = decode_integer(temp_buffer, decInfo);
    return d_success;
}
/* function definition for decoding secret msg (string)*/
Status1 decode_secret_msg(long file_extension_length, DecodeInfo *decInfo)
{
    char str2[1000000];
    /* function call to decode string */
    char *p1 = decode_string(file_extension_length, str2, decInfo);
    strcpy(decInfo->decoded_secret_msg, p1);
    return d_success;
}
