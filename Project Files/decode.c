#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "decode.h"
#include "types.h"
#include "common.h"
#include "msg.h"

Status read_and_validate_decode_args ( char *argv[], DecodeInfo *decodInfo)
{
	if(argv[2] == NULL)
	{
		E(Err_In_img_fn_not_pass);
		return e_failure ;
	}
	decodInfo->src_image_fname = argv[2];
	if(argv[3] == NULL)
	{ 
		I(Default_decode_file);
		decodInfo->secret_fname = "decode";
	}
	else
		decodInfo->secret_fname = argv[3];
	decodInfo->fptr_secret=NULL;
	decodInfo->fptr_src_img = open_file( decodInfo->src_image_fname , "r" );
	if( decodInfo->fptr_src_img == NULL )
		return e_failure;
	return e_success;
}

Status do_decoding ( DecodeInfo *decodInfo)
{
	I(Val_m_str);
	if (check_magic_string ( decodInfo->fptr_src_img) == e_failure)
	{
		E(Err_val_m_str);
		return e_failure;
	}
	S(Suc_val_m_str);
	I(Get_sec_f_extn);
	if ( get_secret_file_extn ( decodInfo ) == e_failure )
	{
		E(Err_get_sec_f_extn);
		return e_failure;
	}
	S(Suc_get_sec_f_extn);
	I(Get_sec_f_data);
	if ( get_secret_data( decodInfo) == e_failure )
	{
		E(Err_get_sec_f_data);
		return e_failure;

	}
	S(Suc_get_sec_f_data);
	close_decode_file (decodInfo);
	return e_success;

}

Status get_secret_data( DecodeInfo *decodInfo)
{
	uint data_size = get_size( decodInfo->fptr_src_img);
	char buffer_input[data_size * 8], buffer_output[data_size];
	fread(buffer_input , 1, data_size * 8, decodInfo->fptr_src_img );
	get_decoded_data ( buffer_input , buffer_output , data_size);
	if ( fwrite( buffer_output , 1, data_size , decodInfo->fptr_secret ) == data_size )
		return e_success;

	return e_failure;

}

Status get_secret_file_extn( DecodeInfo *decodInfo)
{
	uint size = get_size(decodInfo->fptr_src_img);
	if (!( size > 1  && size < 5 ))
		return e_failure;
	get_extension( decodInfo , size);
	decodInfo->fptr_secret = open_file(decodInfo->secret_fname , "w" );
	return ( decodInfo->fptr_secret == NULL ) ? e_failure : e_success ;
}

Status get_extension( DecodeInfo *decodInfo, uint size)
{

	uint sec_fname_size = strlen (decodInfo->secret_fname); 
	char *temp_fname = malloc(sec_fname_size + size);
	char buffer_input[size * 8 ];
	fread ( buffer_input , 1, size * 8, decodInfo->fptr_src_img);
	if ( get_decoded_data( buffer_input , decodInfo-> secret_file_suffix , size) == e_failure )
		return e_failure;
	memcpy (temp_fname , decodInfo->secret_fname, sec_fname_size );
	memcpy(temp_fname + sec_fname_size , decodInfo->secret_file_suffix , size );
	decodInfo->secret_fname = temp_fname ;

	return e_success;
}

uint get_size( FILE *fptr_src)
{
	char buffer_input[32];
	char buffer_output[4];
	fread( buffer_input , 1, 32, fptr_src);
	if ( get_decoded_data (buffer_input, buffer_output, 4) == e_failure )
		return 5;
	uint temp_size ;
	memcpy ( &temp_size , buffer_output , 4);


	return temp_size;
}

Status check_magic_string( FILE *scr_img)
{
	char sec_str[] = MAGIC_STRING;
	fseek(scr_img, 54L, SEEK_SET );
	int secret_msg_len = strlen(sec_str);
	char buffer_input[secret_msg_len * 8], buffer_output[secret_msg_len];
	fread( buffer_input , 1, 16, scr_img);
	if ( get_decoded_data (buffer_input, buffer_output, secret_msg_len) == e_failure )
		return e_failure;

	if ( memcmp ( sec_str , buffer_output,2 ) == 0 )

		return e_success;


	return e_failure;

}

Status get_decoded_data( char *encoded_data, char decoded_data[], uint size)
{
	int i;
	char data[8] ;

	for ( i = 0 ; i < size ; i++ )
	{
		memcpy(data , encoded_data + ( i* 8), 8);
		decoded_data[i] = get_decoded_lsb( data);
	}

	return ( i == size )? e_success : e_failure ;
}

char get_decoded_lsb( char *data)
{
	char ch = 0;
	int temp;
	for ( int i = 0 ; i < 8 ; i++)
	{
		temp = data[i] & 1 ;
		ch = (ch << 1) | temp ;
	}

	//	printf ( "char %c \n",ch);
	return ch;

}

FILE *open_file ( char *fname, char *mode )
{
	FILE *fptr;
	fptr = fopen( fname, mode );
	if ( fptr == NULL )
	{
		perror("ERROR  ");
		fprintf(stderr,"ERROR  : Unable of open file %s\n",fname);
	}
	return fptr;
}

Status close_decode_file (DecodeInfo *decodInfo)
{
	if ( decodInfo->fptr_src_img != NULL )
		fclose (decodInfo->fptr_src_img);
	if ( decodInfo->fptr_secret != NULL )
		fclose (decodInfo->fptr_secret);
	return e_success;
}
