//
// DL ISO8583 library demo
//

#include "dl_iso8583.h"
#include "dl_iso8583_defs_1993.h"
#include "dl_output.h" // for 'DL_OUTPUT_Hex'



int main ( void )
{
    DL_ISO8583_HANDLER isoHandler;
    DL_ISO8583_MSG     isoMsg;
    DL_UINT8           packBuf[1000];
    DL_UINT32          packedSize;
    DL_ERR             err = kDL_ERR_NONE;

    /* get ISO-8583 1993 handler */
    DL_ISO8583_DEFS_1993_GetHandler(&isoHandler);
    
    //
    // Populate/Pack message
    //

    /* initialise ISO message */
    DL_ISO8583_MSG_Init(NULL,0,&isoMsg);

    /* set ISO message fields */
    if (!err) err = DL_ISO8583_MSG_SetField_Str(0,"1234",&isoMsg);
    if (!err) err = DL_ISO8583_MSG_SetField_Str(2,"1234567890123456",&isoMsg);
    if (!err) err = DL_ISO8583_MSG_SetField_Str(4,"5699",&isoMsg);
    if (!err) err = DL_ISO8583_MSG_SetField_Str(11,"234",&isoMsg);
    if (!err) err = DL_ISO8583_MSG_SetField_Str(39,"4",&isoMsg);
    if (!err) err = DL_ISO8583_MSG_SetField_Str(41,"12345",&isoMsg);
    if (!err) err = DL_ISO8583_MSG_SetField_Str(42,"678901234",&isoMsg);
    if (!err) err = DL_ISO8583_MSG_SetField_Str(125,"BLAH BLAH",&isoMsg);

    /* output ISO message content */
    if (!err) DL_ISO8583_MSG_Dump(stdout,NULL,&isoHandler,&isoMsg);

    /* pack ISO message */
    if (!err) err = DL_ISO8583_MSG_Pack(&isoHandler,&isoMsg,packBuf,&packedSize);

    /* free ISO message */
    if (!err) DL_ISO8583_MSG_Free(&isoMsg);

    /* output packed message (in hex) */
    if (!err) DL_OUTPUT_Hex(stdout,NULL,packBuf,packedSize);

    //
    // Unpack message
    //

    /* initialise ISO message */
    if (!err) DL_ISO8583_MSG_Init(NULL,0,&isoMsg);

    /* unpack ISO message */
    if (!err) err = DL_ISO8583_MSG_Unpack(&isoHandler,packBuf,packedSize,&isoMsg);

    /* output ISO message content */
    if (!err) DL_ISO8583_MSG_Dump(stdout,NULL,&isoHandler,&isoMsg);

    /* free ISO message */
    if (!err) DL_ISO8583_MSG_Free(&isoMsg);

    printf("error:%d\n", err);
    
    return 0;
}
