#ifndef MSG_H
#define MSG_H 

/* Error Msg */

#define Err_No_arg_pass  "No argument pass"
#define Err_In_img_fn_not_pass "Input image file name not passed in cmd line argurment"
#define Err_In_secret_msg_fn_not_pass "Input secret file name not passed in cmd line argurment"
#define Err_Max_f_ext_size_exceed "Character size of an secret fille extenion must be less or equal to 4 characters including Dot[.]"
#define Err_Capicty "Size of image file  is  not suciffent to encode secrect text"
#define Err_Opr_ndefine "No such operaion defined"
#define Err_Cp_bmp_head "Someting went worng while coping header file. Please try again"
#define Err_Encd_m_str "Someting went worng while encoding magic string"
#define Err_Sec_f_extn "Someting went worng while encoding secret file extension"
#define Err_Sec_f_size "Someting went worng while encoding secret file size"
#define Err_Sec_f_extn_size "Someting went worng while encoding secret file extension size"
#define Err_Sec_f_data "Someting went worng while encoding secret file data"
#define Err_cp_rem_data "Someting went worng while coping remaning data"

#define Err_val_m_str "The image passed dosen't contain any encoded string"
#define Err_get_sec_f_extn "Someting went worng while decoding secret file extension"
#define Err_get_sec_f_data "Someting went worng while decoding secret file data"



/* Info Msg */

#define Valadating_input "Valadating input data"
#define Encd_start "Encoding image starts"
#define Fech_opr "Fetching operation"
#define Encode_format  "For encode .a/out -e <img.bmp> <sercet_msg_filename> <output.bmp>(optional)"
#define Decode_format  "For decode .a/out -d <img.bmp> <output_filename_without_extension>(optional)" 
#define Decod_start "Decoding image starts"
#define Header_cp "Coping header"
#define Encd_M_str "Encoding Magic string"
#define Sec_f_extn "Encoding Secret file extension"
#define Sec_f_size "Encoding Secret file size"
#define Sec_f_extn_size "Encoding Secret file extension size"
#define Sec_f_data "Encoding Secret file data"
#define Cp_rem_data "Coping remaning data"

#define Val_m_str "Validating magic string"
#define Get_sec_f_extn "Decoding secret file extension"
#define Get_sec_f_data "Decoding Secret file data"
#define Default_decode_file "Default decode file with name <decode.extention_encode_in_src_file> will be created"
#define Default_encode_file "Default encode file with name <encode.bmp> will be created"



/* Sucess msg */

#define Suc_Valadation_cmpt "Valadation complet"
#define Suc_Encd_cmpt "Encoding completed sucessfully"
#define Suc_decode_cmpt "Decoding Completed sucessfully"
#define Suc_Opr_fetch "Operation fetched sucessfully"
#define Suc_Header_cp "Header copied sucessfully"
#define Suc_m_str "Magic string encoded sucessfully"
#define Suc_Sec_f_extn "Secret file extesion encoded sucessfully"
#define Suc_Sec_f_size "Secret file size encoded sucessfully"
#define Suc_Sec_f_extn_size "Secret file extension size encoded sucessfully"
#define Suc_Sec_f_data "Secret file data encoded sucessfully"
#define Suc_Cp_rem_data "copied remaing data sucessfully"


#define Suc_val_m_str "Magic string validated sucessfully"
#define Suc_get_sec_f_extn "Secret file extesion decoded sucessfully"
#define Suc_get_sec_f_data "Secret file data decoded sucessfully"




#endif
