//
// DL ISO8583 library - benchmark test
//

#include "dl_timer.h"

#include "dl_iso8583.h"
#include "dl_iso8583_defs_1987.h"


// indicates the number of pack/unpack iterations performed
#define kITERATIONS             1000000

// indicates the buffer size (in bytes)
#define kBUFFER_SIZE    1000

// if defined then ISO messages will use a static memory buffer, otherwise
// dynamic memory allocation will be used
//#define USE_STATIC_MEMORY 


int main ( void )
{
    DL_TIMER           t;
    DL_ISO8583_HANDLER isoHandler;
    DL_ISO8583_MSG     isoMsg;
    DL_UINT8           buf[kBUFFER_SIZE];
    DL_UINT32          bufSize;
    long               i;
#ifdef USE_STATIC_MEMORY
    DL_UINT8           sBuf[kBUFFER_SIZE];
#endif

    DL_ISO8583_DEFS_1993_GetHandler(&isoHandler);

    /* initialise ISO message */
#ifdef USE_STATIC_MEMORY
    DL_ISO8583_MSG_Init(sBuf,kBUFFER_SIZE,&isoMsg);
#else
    DL_ISO8583_MSG_Init(NULL,0,&isoMsg);
#endif

    /* set ISO message fields */
    (void)DL_ISO8583_MSG_SetField_Str(0,"0800",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(2,"454000000000003",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(3,"000000",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(11,"002001",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(12,"050818174801",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(13,"0818",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(14,"1709",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(15,"0819",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(28,"150818",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(34,"12345ACCNUM",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(41,"12345678",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(42,"123456789012345",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(60,"TEST DATA F60",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(61,"LONGER TEST DATA F61",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(62,"The quick brown fox jumped over the lazy dog F63",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(70,"301",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(128,"01234567",&isoMsg);

    /* output ISO message content */
    DL_ISO8583_MSG_Dump(stdout,NULL,&isoHandler,&isoMsg);

    DL_TIMER_Start(&t);

    for ( i=0 ; i<kITERATIONS ; i++ ) {

        printf("%d\n",i);
        
        (void)DL_ISO8583_MSG_Pack(&isoHandler,&isoMsg,buf,&bufSize);

        /* destroy ISO Msg */
        DL_ISO8583_MSG_Free(&isoMsg);

        /* output packed message (in hex) */
        if (i == 0)
            DL_OUTPUT_Hex(stdout,NULL, buf,bufSize);
            
        /* initialise ISO message (for unpack) */
#ifdef USE_STATIC_MEMORY
        DL_ISO8583_MSG_Init(sBuf,kBUFFER_SIZE,&isoMsg);
#else
        DL_ISO8583_MSG_Init(NULL,0,&isoMsg);
#endif

        (void)DL_ISO8583_MSG_Unpack(&isoHandler,buf,bufSize,&isoMsg);

        /* output ISO message content */
        if (i == 0)
            DL_ISO8583_MSG_Dump(stdout,NULL,&isoHandler,&isoMsg);

    }

    printf("----> TPS = %1.2f\n\n\n",(((float)(kITERATIONS) / (float)DL_TIMER_GetDuration(t)) * 1000));

    /* destroy ISO Msg */
    DL_ISO8583_MSG_Free(&isoMsg);

    return 0;
}
