# LSB_Image_Steganography
This project demonstrates the use of Least Significant Bit (LSB) Steganography to hide secret data within an image file. 
LSB steganography is a technique where the least significant bits of the pixel values in an image are modified to encode hidden data, making the changes invisible to the human eye.
# ENCODING
1. The secret message is converted to its binary representation.
2. For each bit of the message, the LSB of the image pixels is replaced with the corresponding bit of the message.
# DECODING
1. The program retrieves the LSBs of the pixels to reconstruct the hidden binary data, which is then converted back into the original secret message.
# How to Run
1. Compile and run the C program.
2. Provide the input image and the message to be hidden.
3. Extract the hidden message by running the decoding part of the program.
# Applications
1. Secure Communication: 
   ---->LSB steganography is useful for sending confidential information securely.
2. Digital Watermarking:
   ----> It can be used to watermark images to protect copyrights.
