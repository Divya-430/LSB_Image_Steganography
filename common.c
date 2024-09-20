/* Name: K. Divya
   Date: 26/08/2024
   Description: LSB Image Steganography Project
*/
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;

    // checking if argumnet 1 is null
    if (argv[1] == NULL)
    {
        printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [output file]\n");
        printf("./lsb_steg: Decoding: ./lsb_steg -d <.bmp file> [output file]\n");
    }
     // checking if argumnet 1 is not null
    else if (argv[1] != NULL)
    {
         // checking if arg1 is encoding operation 
        if (strcmp(argv[1], "-e") == 0)
        {
            if (argv[2] == NULL)
            {
                printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [output file]\n");
            }
            else if (argv[3] == NULL)
            {
                printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [output file]\n");
            }
            else
            {
                //function call to check  operation type (-e or -d)
                if(argc<=5)
                {
                if (check_operation_type(argv) == e_encode)
                {
                    //function call to read and validate encode args 
                    if (read_and_validate_encode_args(argv, &encInfo) == e_success)
                    {
                        printf("INFO: User selected encoding operation \n");
                        do_encoding(&encInfo);
                    }
                }
                }
                else
                {
                     printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [output file]\n");
                }
            }
        }
        // checking if arg1 is decoding operation 
        else if (strcmp(argv[1], "-d") == 0)
        {
            if (argv[2] == NULL)
            {
                printf("./lsb_steg: Decoding: ./lsb_steg -d <.bmp file> [output file]\n");
            }
            else
            {
                if(argc<=4)
                {
                //function call to check  operation type (-e or -d)
                if (check_operation_type(argv) == e_decode)
                {
                     //function call to read and validate encode args 
                    if (read_and_validate_decode_args(argv, &decInfo) == e_success)
                    {
                        printf("INFO: User selected Decoding operation \n");
                        do_decoding(&decInfo);
                    }
                    else
                    {
                       printf("./lsb_steg: Decoding: ./lsb_steg -d <.bmp file> [output file]\n");
                    }
                }
                }
                else
                {
                    printf("./lsb_steg: Decoding: ./lsb_steg -d <.bmp file> [output file]\n");
                }
            }
        }
        else
        {
            printf("INFO: Unsupported operation \n");
        }
    }
    return 0;
}
/*function definition for check_operation_type */
OperationType check_operation_type(char *argv[])
{
    /* if :checking argv[1] ==> -e */
    if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    /* else if : argv[1] ==> -d */
    else if (strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    /* else return e_unsupported */
    else
    {
        return e_unsupported;
    }
}