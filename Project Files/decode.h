#ifndef DECODE_H
#define DECODE_H

#include "types.h"

#define MAX_SECRECT_FILE_SUFFIX 5

typedef struct _DecodeInfo
{
	char *src_image_fname;
	FILE *fptr_src_img;

	char *secret_fname;
	FILE *fptr_secret;
	char secret_file_suffix[MAX_SECRECT_FILE_SUFFIX];
} DecodeInfo;

// to get and validate data for decoding file.
Status read_and_validate_decode_args( char *argv[] , DecodeInfo *decodInfo);

// to get file ptr of any file
FILE *open_file ( char *fname, char *mode);

// to start decoding 
Status do_decoding ( DecodeInfo *decodInfo);

// check magic string match
Status check_magic_string ( FILE *scr_img );

// to get extension of secret file
Status get_secret_file_extn ( DecodeInfo *decodInfo);

// to decode extension
Status get_extension ( DecodeInfo *decodInfo, uint size);

// to get size stored in integer format
uint get_size(  FILE *fptr_src);

// to get secret  data
Status get_secret_data( DecodeInfo *decodInfo);

// to get decoded data
Status get_decoded_data ( char *encoded_data, char *decoded_data, uint size);

// to fetch data grom lsb
char get_decoded_lsb ( char *data);

// close all file
Status close_decode_file( DecodeInfo *decodInfo);
#endif
