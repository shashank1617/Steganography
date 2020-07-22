#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "encode.h"
#include "types.h"
#include "msg.h"
#include "common.h"


OperationType check_operation_type (char *argv[])
{
	if ( strcmp (argv[1], "-e") == 0 || strcmp (argv[1], "-E") == 0 )
		return e_encode;
	else if ( strcmp (argv[1], "-d") == 0 || strcmp (argv[1], "-D") == 0 )
		return e_decode;

	return e_unsupported;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
	if ( argv[2] == NULL )
	{
		E(Err_In_img_fn_not_pass);
		return e_failure;
	}

	if ( argv[3] == NULL )
	{
		E(Err_In_secret_msg_fn_not_pass);
		return e_failure;
	}

	encInfo->src_image_fname = argv[2];
	encInfo->secret_fname = argv[3];

	if ( argv[4] == NULL )
	{
		I(Default_encode_file);
		encInfo->stego_image_fname = "encode.bmp";
	}
	else
		encInfo-> stego_image_fname = argv[4] ;

	if ( open_files (encInfo) == e_failure )
	{
		return e_failure;
	}

	encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
	encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

	char *file_ext = strrchr(encInfo->secret_fname, '.');
	
	if( strlen(file_ext) >= MAX_FILE_SUFFIX )
	{                             
		E(Err_Max_f_ext_size_exceed);
		return e_failure;
	}

	strcpy(encInfo->extn_secret_file, file_ext);

	if( check_capacity(encInfo) == e_failure )
	{
		E(Err_Capicty);
		return e_failure;
	}

	return e_success;
}


Status do_encoding (EncodeInfo *encInfo)
{
	I(Header_cp);
	if ( copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
	{
		E(Err_Cp_bmp_head);
		return e_failure;
	}
		S(Suc_Header_cp);
	char *magic_string = MAGIC_STRING;
	I(Encd_M_str);
	if( encode_magic_string( magic_string , encInfo) == e_failure )
	{
		E(Err_Encd_m_str);
		return e_failure;
	}
	S(Suc_m_str);
	if ( encode_secret_file_extn ( encInfo->extn_secret_file, encInfo) == e_failure)
		return e_failure;
	I(Sec_f_size);
	if ( encode_secret_file_size ( encInfo->size_secret_file, encInfo ) == e_failure)
	{
		E(Err_Sec_f_size);
		return e_failure;
	}
	S(Suc_Sec_f_size);
	I(Sec_f_data);
	if (encode_secret_file_data (encInfo) == e_failure )
	{
		E(Err_Sec_f_data);
		return e_failure;
	}
	S(Suc_Sec_f_data);
	I(Cp_rem_data);
	if ( copy_remaining_img_data( encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
	{
		E(Err_cp_rem_data);
		return e_failure;
	}
	S(Suc_Cp_rem_data);

	close_encod_files (encInfo);
	return e_success;
}

Status copy_bmp_header( FILE *fptr_src_image, FILE *fptr_dest_image)
{
	fseek( fptr_dest_image , 0, SEEK_SET);
	fseek( fptr_src_image, 0, SEEK_SET);
	char header_data[54];
	if( fread (header_data,1,54,fptr_src_image) == 54 )
	{
	if ( fwrite(header_data, 1, 54, fptr_dest_image) == 54 )
		return e_success;
	}
	return e_failure;
}

Status encode_magic_string( const char *magic_string, EncodeInfo *encInfo)
{
	if(	encode_data_to_image ((char *) magic_string , strlen(magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure )
	{
		return e_failure;
	}
	return e_success;
}

Status encode_secret_file_extn ( const char *file_extn, EncodeInfo *encInfo)
{
	char len[4];
	uint size = strlen(encInfo->extn_secret_file);
	memcpy (len, &size, 4 );
	I(Sec_f_extn_size);
	if(	encode_data_to_image (len , 4, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure )
	{
		E(Err_Sec_f_extn_size);
		return e_failure;
	}
	S(Suc_Sec_f_extn_size);
	I(Sec_f_extn);
	if(	encode_data_to_image ((char *)file_extn , strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure )
	{
		E(Err_Sec_f_extn);
		return e_failure;
	}
	S(Suc_Sec_f_extn);
	return e_success;
}

Status encode_secret_file_size( long file_size, EncodeInfo *encInfo )
{
	char len[4];
	memcpy (len , &(encInfo->size_secret_file) , 4 );
	if(	encode_data_to_image (len , 4, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure )
	{
		return e_failure;
	}
	return e_success;
}

Status encode_secret_file_data (EncodeInfo *encInfo)
{
	char file_data[encInfo->size_secret_file];
	fseek( encInfo->fptr_secret, 0 , SEEK_SET );
	fread( file_data, 1, encInfo->size_secret_file, encInfo->fptr_secret);
	if(	encode_data_to_image (file_data , encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure )
	{
		return e_failure;
	}
	return e_success;
}

Status copy_remaining_img_data( FILE *fptr_src, FILE *fptr_dest)
{
	uint curr_pos = ftell(fptr_src);
	fseek( fptr_src, 0, SEEK_END);
	uint rem_size = ftell(fptr_src) - curr_pos ;
	fseek ( fptr_src, curr_pos, SEEK_SET );
	char image_buffer[rem_size];
	if ( fread(image_buffer, 1, rem_size, fptr_src ) == rem_size )
	{
		if(	fwrite( image_buffer, 1, rem_size, fptr_dest ) == rem_size )
			return e_success;
	}
	return e_failure ;
}

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image )
{ 
	char image_buffer[8];
	for ( int i = 0 ; i < size ; i++ )
	{
		if ( fread( image_buffer, 1, 8, fptr_src_image) != 8 )
			return e_failure;
		if (encode_byte_tolsb ( *data , image_buffer ) == e_failure )
			return e_failure;
		if ( fwrite( image_buffer, 1, 8, fptr_stego_image ) != 8)
			return e_failure;
		data++;
	}
	return  e_success ;
}

Status encode_byte_tolsb( char data, char *image_buffer)
{
	uint temp;
	int i ;
	for ( i = 0 ; i < 8 ; i++ )
	{
		temp = (data >> (7 - i)) & 1;
		if(temp == 1)
			image_buffer[i] = image_buffer[i] | temp ;
		else
			image_buffer[i] = image_buffer[i] & ~1 ;
	}
	return ( i == 8 ) ? e_success : e_failure ;
}

uint get_image_size_for_bmp(FILE *fptr_image)
{
	uint width, height;
	fseek(fptr_image, 18, SEEK_SET);
	fread(&width, sizeof(int), 1, fptr_image);
	fread(&height, sizeof(int), 1, fptr_image);
	return width * height * 3;
}

uint get_file_size(FILE *fptr)
{
	uint cur_pos = ftell(fptr);
	fseek(fptr,0,SEEK_END);
	uint size = ftell(fptr);
	fseek(fptr,cur_pos,SEEK_SET);
	return size;
}

Status check_capacity( EncodeInfo *encInfo)
{
	uint req_size = 80 + ( (encInfo->size_secret_file + strlen(encInfo->extn_secret_file)) * 8) ;

	if ( encInfo->image_capacity >= req_size )
		return e_success;

	return e_failure;
}

Status open_files(EncodeInfo *encInfo)
{
	encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");

	if (encInfo->fptr_src_image == NULL)
	{
		perror("ERROR  ");
		fprintf(stderr, "ERROR  : Unable to open file %s\n", encInfo->src_image_fname);
		return e_failure;
	}

	encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
	if (encInfo->fptr_secret == NULL)
	{
		perror("ERROR  ");
		fprintf(stderr, "ERROR  : Unable to open file %s\n", encInfo->secret_fname);
		return e_failure;
	}

	encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
	if (encInfo->fptr_stego_image == NULL)
	{
		perror("ERROR  ");
		fprintf(stderr, "ERROR  : Unable to open file %s\n", encInfo->stego_image_fname);
		return e_failure;
	}

	return e_success;
}

Status close_encod_files ( EncodeInfo *encInfo)
{
	if ( encInfo->fptr_src_image != NULL)
		fclose ( encInfo->fptr_src_image);

	if ( encInfo->fptr_secret != NULL)
		fclose ( encInfo->fptr_secret);

	if ( encInfo->fptr_stego_image != NULL)
		fclose ( encInfo->fptr_stego_image);

	return e_success;
}
