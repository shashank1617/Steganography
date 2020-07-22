#ifndef TYPES_H
#define TYPES_H

#define I(msg) printf("INFO   : %s \n",msg)
#define S(msg) printf("SUCESS : %s \n",msg)
#define E(msg) printf("ERROR  : %s \n",msg)

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_encode,
    e_decode,
    e_unsupported
} OperationType;


#endif
