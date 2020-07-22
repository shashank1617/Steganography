#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "encode.h"
#include "decode.h"
#include "msg.h"



int main( int argc , char **args)
{
	
	if ( argc < 2 )
	{
		E(Err_No_arg_pass);
		I(Encode_format);
		I(Decode_format);
		exit (EXIT_FAILURE);
	}

	I(Fech_opr);
	OperationType operation = check_operation_type (args);
	S(Suc_Opr_fetch);
	I(Valadating_input);

	switch(operation)
	{
		case e_encode :
			{
				EncodeInfo encode_info;
				if ( read_and_validate_encode_args ( args, &encode_info) == e_failure )
					exit(EXIT_FAILURE);

				S(Suc_Valadation_cmpt);
				I(Encd_start);

				(do_encoding( &encode_info) == e_success)? S(Suc_Encd_cmpt) : close_encod_files( &encode_info) , exit(EXIT_FAILURE);
			}
			break;
		case e_decode :
			{
				DecodeInfo decode_info;
				if ( read_and_validate_decode_args ( args, &decode_info ) == e_failure )
					exit(EXIT_FAILURE);

				S(Suc_Valadation_cmpt);
				I(Decod_start);
				(do_decoding (&decode_info) == e_success ) ? S(Suc_decode_cmpt) :  close_decode_file(&decode_info) , exit(EXIT_FAILURE) ;
			}
			break;
		default :
			{
				E(Err_Opr_ndefine);
				exit(EXIT_FAILURE);
			}
	}

	return 0;
}
