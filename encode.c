/* Name: K. Divya
   Date: 26/08/2024
   Description: LSB Image Steganography Project
*/
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);
    rewind(fptr_image);

    // Return image capacity
    return width * height * 3;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }
    // No failure return e_success
    return e_success;
}
/* function definition for read_and_validation_encode_args */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    char *ptr;
    /*checking whether argv[2] contains substring as .bmp
    If it contains, then store file name into src_image_fname  else return failure */
    char *str = strstr(argv[2], ".");
    if (strcmp(str, ".bmp") == 0)
    {
        encInfo->src_image_fname = argv[2];
    }
    else
    {
        printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [output file]\n");
        return e_failure;
    }

    /*checking whether argv[3] contains substring as "." (any type of extension file)
    If it contains, then store file name into secret_fname  else return failure */
    if (ptr = strstr(argv[3], "."))
    {
        encInfo->secret_fname = argv[3];
        strcpy(encInfo->extn_secret_file, ptr);
    }
    else
    {
        printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [output file]\n");
        return e_failure;
    }
    if (argv[4] != NULL)
    {
        /*checking whether argv[4] contains substring as .bmp
        If it contains, then store file name into stego_image_fname  else return failure */
        char *str = strstr(argv[4], ".");
        if (str == NULL)
        {
            printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [output file]\n");
            return e_failure;
        }
        if (strcmp(str, ".bmp") == 0)
        {
            encInfo->stego_image_fname = argv[4];
        }
        else
        {
            printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [output file]\n");
            return e_failure;
        }
    }
    else
    {
        printf("INFO: Output File not mentioned. Creating deafult_stego.bmp as default \n");
        encInfo->stego_image_fname = "default_stego.bmp";
    }
    return e_success;
}
/* read magic string from user */
char magic_string[10];
Status do_encoding(EncodeInfo *encInfo)
{
    /* call open files function*/
    printf("INFO: Opening required files \n");
    if (open_files(encInfo) == e_failure)
    {
        return e_failure;
    }
    printf("INFO: Opened %s \n", encInfo->src_image_fname);
    printf("INFO: Opened %s \n", encInfo->secret_fname);
    printf("INFO: Done \n");

    printf("Enter the magic string of of size less than 10 :");
    scanf("%[^\n]", magic_string);
    getchar();
    if (strlen(magic_string) >= 10)
    {
        printf("INFO: Magic string length is exceeded \n");
        return e_failure;
    }

    printf("INFO: ## Encoding procedure started ## \n");
    printf("INFO: Checking for %s size \n", encInfo->secret_fname);
    int res = get_file_size(encInfo->fptr_secret);
    if (res > 0)
    {
        printf("INFO: Done. Not Empty \n");
    }
    else
    {
        printf("INFO: File is Empty \n");
        return e_failure;
    }

    /* check capacity */
    printf("INFO: Checking for %s capacity to handle %s \n", encInfo->src_image_fname, encInfo->secret_fname);
    int ret = check_capacity(encInfo);
   

    if (ret == e_success)
    {
        printf("INFO: Done. Found OK \n");
        encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
        // Do Error handling
        if (encInfo->fptr_stego_image == NULL)
        {
            perror("fopen");
            fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

            return e_failure;
        }
         /* opening output file after checking the capacity */
    printf("INFO: Opened %s \n", encInfo->stego_image_fname);
        printf("INFO: Copying Image Header \n");
        /* copy bmp header */
        int ret1 = copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image);
        if (ret1 == e_success)
        {
            printf("INFO: Done \n");
        }
        /* encode the length of magic string (integer)*/
        printf("INFO: Encoding Magic String Size \n");
        int ret2 = encode_magic_string_size(strlen(magic_string), encInfo);
        if (ret2 == e_success)
        {
            printf("INFO: Done \n");
        }

        /* encode the magic string (string)*/
        printf("INFO: Encoding Magic String Signature \n");
        int ret3 = encode_magic_string(magic_string, encInfo);
        if (ret3 == e_success)
        {
            printf("INFO: Done \n");
        }

        /* encode the length of  secret msg extension */
        printf("INFO: Encoding %s extension length \n", encInfo->secret_fname);
        int ret4 = encode_extn_size(strlen(encInfo->extn_secret_file), encInfo);
        if (ret4 == e_success)
        {
            printf("INFO: Done \n");
        }

        /* encode the secret msg extension */
        printf("INFO: Encoding  %s File Extension \n", encInfo->secret_fname);
        int ret5 = encode_secret_file_extn(encInfo->extn_secret_file, encInfo);
        if (ret5 == e_success)
        {
            printf("INFO: Done \n");
        }

        /* encode the length of secret msg */
        printf("INFO: Encoding  %s file Size \n", encInfo->secret_fname);
        int ret6 = encode_secret_file_size(encInfo->size_secret_file, encInfo);
        if (ret6 == e_success)
        {
            printf("INFO: Done \n");
        }

        /* encode the secret msg */
        printf("INFO: Encoding %s File Data \n", encInfo->secret_fname);
        int ret7 = encode_secret_file_data(encInfo);
        if (ret7 == e_success)
        {
            printf("INFO: Done \n");
        }

        /* copy the remaining data*/
        printf("INFO: Copying Left Over Data \n");
        int ret8 = copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image);
        if (ret8 == e_success)
        {
            printf("INFO: Done \n");
        }
        printf("INFO: ## Encoding Done successfully ##\n");
    } /* close all the opened files */
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);

    return e_success;
}

/* get file size*/
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    ftell(fptr);
}
/* function definition to check_capacity */
Status check_capacity(EncodeInfo *encInfo)
{
    /* ->check if Image capacity greater than encoding things */
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    printf("File size : %ld\n", encInfo->size_secret_file);
    int encode_size = 54 + ((4 + strlen(magic_string)) + 4 + (strlen(encInfo->extn_secret_file)) + 4 + (encInfo->size_secret_file)) * 8;
    if (encInfo->image_capacity > encode_size)
    {
        printf("INFO: Image capacity is greater than the encoding size \n");
        return e_success;
    }
    else
    {
        printf("INFO: Image capacity is less.It can't handle the data \n");
        return e_failure;
    }
}

/* function definition to copy_bmp_header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    /* copying the 54 bytes from source file to output file */
    char buffer[54];
    fread(buffer, 54, 1, fptr_src_image);
    fwrite(buffer, 54, 1, fptr_dest_image);
    return e_success;
}

/* function definition to encode _magic_string_size (integer)*/
Status encode_magic_string_size(long magic_string_size, EncodeInfo *encInfo)
{
    /* declare one temp_buffer of size 32 */
    char temp_buffer[32];
    /* read 32 bytes from source file and store into temp_buffer */
    fread(temp_buffer, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb(magic_string_size, temp_buffer);
    /* write 32 bytes from buffer to output file */
    fwrite(temp_buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

/* function definition to encode_size_to_lsb (integer) */
Status encode_size_to_lsb(long data, char *image_buffer)
{
    int bit;
    int mask;
    for (int i = 0; i < 32; i++)
    {
        // get bit from data
        bit = (data >> i) & 1;
        // clear bit
        mask = image_buffer[i] & ~1;
        image_buffer[i] = bit | mask;
    }
    return e_success;
}

/* function definition to encode_magic_string* (string) */
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    /* encode_string_to_image ()*/
    encode_string_to_image(magic_string, strlen(magic_string), encInfo);
    return e_success;
}

/* function definition to encode_string_to_image (string) */
Status encode_string_to_image(char *data, int size, EncodeInfo *encInfo)
{
    /*  declare the temp_buffer of size 8 */
    char temp_buffer[8];
    int bit2, mask2;
    for (int i = 0; i < size; i++)
    {
        fread(temp_buffer, 8, 1, encInfo->fptr_src_image);
        for (int j = 0; j < 8; j++)
        {
            bit2 = (data[i] >> j) & 1;
            temp_buffer[j] = temp_buffer[j] & ~1;
            temp_buffer[j] = bit2 | temp_buffer[j];
        }

        fwrite(temp_buffer, 8, 1, encInfo->fptr_stego_image);
    }
    return e_success;
}

/* function definition to encode secret file extension size (integer) */
Status encode_extn_size(long extn_size, EncodeInfo *encInfo)
{
    char temp_buffer[32];
    /* read 32 bytes from secret  file and store into temp_buffer */
    fread(temp_buffer, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb(extn_size, temp_buffer);
    /* write 32 bytes from buffer to output file */
    fwrite(temp_buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}
/* function definition to encode secret file extension (string)*/
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    encode_string_to_image(file_extn, strlen(file_extn), encInfo);
    return e_success;
}

/* function definition for encoding secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char temp_buffer[32];
    /* read 32 bytes from source file and store into temp_buffer */
    fread(temp_buffer, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb(file_size, temp_buffer);
    /* write 32 bytes from buffer to output file */
    fwrite(temp_buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

/* function definition for encoding secret file  */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char ch;
    int i = 0;
    fseek(encInfo->fptr_secret, 0, SEEK_SET);
    while ((ch = fgetc(encInfo->fptr_secret)) != EOF)
    {
        encInfo->secret_message[i] = ch;
        i++;
    }
    rewind(encInfo->fptr_secret);
    // printf("secret message is : %s\n", encInfo->secret_message);
    encode_string_to_image(encInfo->secret_message, strlen(encInfo->secret_message), encInfo);
    return e_success;
}

/* function definition for copying remainind data */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char temp_buffer[1000];
    uint size;
    /* read from source image to temp buffer and store it in a variable*/
    while ((size = fread(temp_buffer, 1, sizeof(temp_buffer), fptr_src)) > 0)
    {
        if (fwrite(temp_buffer, 1, size, fptr_dest) != size)
        {
            return e_failure;
        }
    }
    return e_success;
}
