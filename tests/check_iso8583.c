#include <config.h>
#include <stdlib.h>
#include <stdint.h>
#include <check.h>
#include "dl_iso8583.h"
#include "dl_output.h"
#include "dl_iso8583_defs_1993.h"

static DL_UINT8 E2A[256] ={
    0x00, 0x01, 0x02, 0x03, 0x9C, 0x09, 0x86, 0x7F,
    0x97, 0x8D, 0x8E, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x9D, 0x85, 0x08, 0x87,
    0x18, 0x19, 0x92, 0x8F, 0x1C, 0x1D, 0x1E, 0x1F,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x0A, 0x17, 0x1B,
    0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x05, 0x06, 0x07,
    0x90, 0x91, 0x16, 0x93, 0x94, 0x95, 0x96, 0x04,
    0x98, 0x99, 0x9A, 0x9B, 0x14, 0x15, 0x9E, 0x1A,
    0x20, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6,
    0xA7, 0xA8, 0xD5, 0x2E, 0x3C, 0x28, 0x2B, 0x7C,
    0x26, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0x21, 0x24, 0x2A, 0x29, 0x3B, 0x5E,
    0x2D, 0x2F, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
    0xB8, 0xB9, 0xE5, 0x2C, 0x25, 0x5F, 0x3E, 0x3F,
    0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1,
    0xC2, 0x60, 0x3A, 0x23, 0x40, 0x27, 0x3D, 0x22,
    0xC3, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
    0x68, 0x69, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9,
    0xCA, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,
    0x71, 0x72, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0,
    0xD1, 0x7E, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
    0x79, 0x7A, 0xD2, 0xD3, 0xD4, 0x5B, 0xD6, 0xD7,
    0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0x5D, 0xE6, 0xE7,
    0x7B, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
    0x48, 0x49, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED,
    0x7D, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
    0x51, 0x52, 0xEE, 0xEF, 0xF0, 0xF1, 0xF2, 0xF3,
    0x5C, 0x9F, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
    0x59, 0x5A, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

static DL_UINT8 A2E[256] ={
    0x00, 0x01, 0x02, 0x03, 0x37, 0x2D, 0x2E, 0x2F,
    0x16, 0x05, 0x25, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x3C, 0x3D, 0x32, 0x26,
    0x18, 0x19, 0x3F, 0x27, 0x1C, 0x1D, 0x1E, 0x1F,
    0x40, 0x5A, 0x7F, 0x7B, 0x5B, 0x6C, 0x50, 0x7D,
    0x4D, 0x5D, 0x5C, 0x4E, 0x6B, 0x60, 0x4B, 0x61,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
    0xF8, 0xF9, 0x7A, 0x5E, 0x4C, 0x7E, 0x6E, 0x6F,
    0x7C, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
    0xC8, 0xC9, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6,
    0xD7, 0xD8, 0xD9, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6,
    0xE7, 0xE8, 0xE9, 0xAD, 0xE0, 0xBD, 0x5F, 0x6D,
    0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
    0x97, 0x98, 0x99, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6,
    0xA7, 0xA8, 0xA9, 0xC0, 0x4F, 0xD0, 0xA1, 0x07,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x15, 0x06, 0x17,
    0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x09, 0x0A, 0x1B,
    0x30, 0x31, 0x1A, 0x33, 0x34, 0x35, 0x36, 0x08,
    0x38, 0x39, 0x3A, 0x3B, 0x04, 0x14, 0x3E, 0xE1,
    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x58, 0x59, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
    0x68, 0x69, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75,
    0x76, 0x77, 0x78, 0x80, 0x8A, 0x8B, 0x8C, 0x8D,
    0x8E, 0x8F, 0x90, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E,
    0x9F, 0xA0, 0xAA, 0xAB, 0xAC, 0x4A, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
    0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0x6A, 0xBE, 0xBF,
    0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xDA, 0xDB,
    0xDC, 0xDD, 0xDE, 0xDF, 0xEA, 0xEB, 0xEC, 0xED,
    0xEE, 0xEF, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

#define phex(buf, len) {int _i_ = 0; for(_i_=0;_i_<len;_i_++)printf("%02X", buf[_i_]);printf("\n");}

extern DL_ERR _packAscii(DL_UINT8 *iDataPtr, DL_UINT32 iDataLen, DL_UINT32 iOutLen, DL_UINT8 **ioPtr);

void setup(void)
{
    
}

void teardown(void)
{
    
}

START_TEST(test_1)
{
    DL_ISO8583_HANDLER isoHandler;
    DL_ISO8583_MSG     isoMsg;
    DL_UINT8           packBuf[1000];
    DL_UINT32          packedSize;


    /* get ISO-8583 1993 handler */
    DL_ISO8583_DEFS_1993_GetHandler(&isoHandler);
    
    //
    // Populate/Pack message
    //

    /* initialise ISO message */
    DL_ISO8583_MSG_Init(NULL,0,&isoMsg);

    /* set ISO message fields */
    (void)DL_ISO8583_MSG_SetField_Str(0,"1234",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(2,"1234567890123456",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(4,"5699",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(11,"234",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(39,"4",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(41,"12345",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(42,"678901234",&isoMsg);
    (void)DL_ISO8583_MSG_SetField_Str(125,"BLAH BLAH",&isoMsg);

    /* output ISO message content */
    DL_ISO8583_MSG_Dump(stdout,NULL,&isoHandler,&isoMsg);

    /* pack ISO message */
    (void)DL_ISO8583_MSG_Pack(&isoHandler,&isoMsg,packBuf,&packedSize);

    /* free ISO message */
    DL_ISO8583_MSG_Free(&isoMsg);

    /* output packed message (in hex) */
    DL_OUTPUT_Hex(stdout,NULL,packBuf,packedSize);

    //
    // Unpack message
    //

    /* initialise ISO message */
    DL_ISO8583_MSG_Init(NULL,0,&isoMsg);

    /* unpack ISO message */
    (void)DL_ISO8583_MSG_Unpack(&isoHandler,packBuf,packedSize,&isoMsg);
    
    /* output ISO message content */
    DL_ISO8583_MSG_Dump(stdout,NULL,&isoHandler,&isoMsg);

    /* free ISO message */
    DL_ISO8583_MSG_Free(&isoMsg);



    ck_assert_str_eq("USD", "USD");
}
END_TEST

START_TEST(test_2)
{
    int *m = NULL;
    ck_assert_msg(m == NULL,
                  "NULL should be returned on attempt to create with "
                  "a invalid value");
}
END_TEST

START_TEST(test_pack_ascii)
{
    DL_UINT8 szStr[] = "hello';'13 , ";
    DL_UINT32 iOutLen = 20;
    DL_UINT8 buf[21];
    DL_UINT8 *p = buf;
    DL_UINT8 puf[32];
    DL_UINT8 *ppuf = puf;
    
    buf[iOutLen] = 0;
    
    printf("pack ascii [%s],[%d]\n", szStr, strlen(szStr));
    _packAscii(szStr, strlen(szStr), iOutLen, &p);
    phex(buf, iOutLen);
    ck_assert_str_eq(buf, "hello';'13 ,        ");
    ck_assert_str_eq(szStr, "hello';'13 , ");
    ck_assert_ptr_eq(p, buf + iOutLen);

    //unpack
    printf("unpacking...\n");
    p -= iOutLen;
    _unpackAscii(&p, iOutLen, &ppuf);
    phex(puf, iOutLen);
    ck_assert_uint_eq(memcmp(puf, szStr, strlen(szStr)), 0);
    ck_assert_uint_eq(memcmp(buf + strlen(szStr), "                          ", iOutLen - strlen(szStr)), 0);
    ck_assert_ptr_eq(ppuf, puf + iOutLen);
}
END_TEST

START_TEST(test_pack_ebcdic)
{
    DL_UINT8 szStr[] = "hello';'13 , ";
    DL_UINT8 szFinal[] = {
        A2E['h'],  A2E['e'], A2E['l'],  A2E['l'], A2E['o'],
        A2E['\''], A2E[';'], A2E['\''], A2E['1'], A2E['3'],
        A2E[' '],  A2E[','], A2E[' '],  A2E[' '], A2E[' '],
        A2E[' '],  A2E[' '], A2E[' '],  A2E[' '], A2E[' ']
    };
    DL_UINT32 iOutLen = 20;
    DL_UINT8 buf[21];
    DL_UINT8 *p = buf;
    
    buf[iOutLen] = 0;
    printf("pack ebcdic [%s],[%d]\n", szStr, strlen(szStr));    
    _packEbcdic(szStr, strlen(szStr), iOutLen, &p);
    phex(buf, iOutLen);
    ck_assert_uint_eq(memcmp(buf, szFinal, iOutLen), 0);
    ck_assert_str_eq(szStr, "hello';'13 , ");
    ck_assert_ptr_eq(p, buf + iOutLen);

    DL_UINT8 puf[32];
    DL_UINT8 *ppuf = puf;
    //unpack
    printf("unpacking...\n");
    p -= iOutLen;
    _unpackEbcdic(&p, iOutLen, &ppuf);
    phex(puf, iOutLen);
    ck_assert_uint_eq(memcmp(puf, "hello';'13 ,        ", strlen(szStr)), 0);
    ck_assert_ptr_eq(ppuf, puf + iOutLen);    
}
END_TEST

START_TEST(test_pack_byte)
{
    DL_UINT8 szStr[20] = "hello';'13 , ";
    DL_UINT8 szFinal[20] = "hello';'13 , ";
    DL_UINT32 iOutLen = 20;
    DL_UINT8 buf[21];
    DL_UINT8 *p = buf;
    
    buf[iOutLen] = 0;

    printf("pack byte [%s],[%d]\n", szStr, strlen(szStr));
    _packByte(szStr, strlen(szStr), iOutLen, &p);
    phex(buf, iOutLen);

    ck_assert_uint_eq(memcmp(buf, szFinal, iOutLen) ,0);
    ck_assert_str_eq(szStr, "hello';'13 , ");
    ck_assert_ptr_eq(p, buf + iOutLen);

    DL_UINT8 puf[32];
    DL_UINT8 *ppuf = puf;
    //unpack
    printf("unpacking...\n");
    p -= iOutLen;
    _unpackByte(&p, iOutLen, &ppuf);
    phex(puf, iOutLen);
    ck_assert_uint_eq(memcmp(puf, szStr, iOutLen), 0);
    ck_assert_ptr_eq(ppuf, puf + iOutLen);    
}
END_TEST

START_TEST(test_pack_bcd_left)
{
    DL_UINT8 szStr[20] = "612345678901245";
    DL_UINT8 szFinal[] = {
        0x61, 0x23, 0x45, 0x67, 0x89, 0x01, 0x24, 0x50, 0x00, 0x00
    };
    DL_UINT32 iOutLen = 20;
    DL_UINT8 buf[21];
    DL_UINT8 *p = buf;
    DL_UINT32 i;
    
    buf[iOutLen] = 0;
    printf("pack bcd left [%s],[%d]\n", szStr, strlen(szStr));
    _packBcdLeft(szStr, strlen(szStr), iOutLen, &p);
    phex(buf, iOutLen / 2);
    ck_assert_uint_eq(memcmp(buf, szFinal, iOutLen / 2), 0);

    ck_assert_str_eq(szStr, "612345678901245");
    ck_assert_ptr_eq(p, buf + iOutLen / 2);

    DL_UINT8 puf[32];
    DL_UINT8 *ppuf = puf;
    //unpack
    printf("unpacking...\n");
    p -= (iOutLen + 1) / 2;
    _unpackBcdLeft(&p, iOutLen, &ppuf);
    phex(puf, iOutLen);
    ck_assert_uint_eq(memcmp(puf, "61234567890124500000", iOutLen), 0);
    ck_assert_ptr_eq(ppuf, puf + iOutLen);    
}
END_TEST



START_TEST(test_pack_bcd_right)
{
    DL_UINT8 szStr[20] = "612345678901245";
    DL_UINT8 szFinal[] = {
        0x00, 0x00, 0x06, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x45
    };
    DL_UINT32 iOutLen = 20;
    DL_UINT8 buf[21];
    DL_UINT8 *p = buf;
    
    buf[iOutLen] = 0;
    printf("pack bcd right [%s],[%d]\n", szStr, strlen(szStr));    
    _packBcdRight(szStr, strlen(szStr), iOutLen, &p);
    phex(buf, iOutLen / 2);
    ck_assert_uint_eq(memcmp(buf, szFinal, iOutLen / 2), 0);
    ck_assert_str_eq(szStr, "612345678901245");
    ck_assert_ptr_eq(p, buf + iOutLen / 2);
    
    DL_UINT8 puf[32];
    DL_UINT8 *ppuf = puf;
    //unpack
    printf("unpacking...\n");
    p -= (iOutLen + 1) / 2;
    _unpackBcdRight(&p, iOutLen, &ppuf);
    phex(puf, iOutLen);
    ck_assert_uint_eq(memcmp(puf, "00000612345678901245", iOutLen), 0);
    ck_assert_ptr_eq(ppuf, puf + iOutLen);
}
END_TEST

START_TEST(test_pack_nibble_left)
{
    DL_UINT8 szStr[20] = "ABF3456C890D24F";
    DL_UINT8 szFinal[] = {
        0xAB, 0xF3, 0x45, 0x6C, 0x89, 0x0D, 0x24, 0xF0, 0x00, 0x00
    };
    DL_UINT32 iOutLen = 20;
    DL_UINT8 buf[21];
    DL_UINT8 *p = buf;
    
    buf[iOutLen] = 0;
    printf("pack nibble left [%s],[%d]\n", szStr, strlen(szStr));    
    _packNibbleLeft(szStr, strlen(szStr), iOutLen, &p);
    phex(buf, iOutLen / 2);
    ck_assert_uint_eq(memcmp(buf, szFinal, iOutLen / 2), 0);
    ck_assert_str_eq(szStr, "ABF3456C890D24F");
    ck_assert_ptr_eq(p, buf + iOutLen / 2);

    DL_UINT8 puf[32];
    DL_UINT8 *ppuf = puf;
    //unpack
    printf("unpacking...\n");
    p -= (iOutLen + 1) / 2;
    _unpackNibbleLeft(&p, iOutLen, &ppuf);
    phex(puf, iOutLen);
    ck_assert_uint_eq(memcmp(puf, "ABF3456C890D24F00000", strlen(szStr)), 0);
    ck_assert_ptr_eq(ppuf, puf + iOutLen);    
}
END_TEST

START_TEST(test_pack_nibble_right)
{
    DL_UINT8 szStr[] = "ABF3456C890D24F";
    DL_UINT8 szFinal[] = {
        0x00, 0x00, 0x0A, 0xBF, 0x34, 0x56, 0xC8, 0x90, 0xD2, 0x4F
    };
    DL_UINT32 iOutLen = 20;
    DL_UINT8 buf[21];
    DL_UINT8 *p = buf;
    
    buf[iOutLen] = 0;
    printf("pack nibble right [%s],[%d]\n", szStr, strlen(szStr));    
    _packNibbleRight(szStr, strlen(szStr), iOutLen, &p);
    phex(buf, iOutLen / 2);
    ck_assert_uint_eq(memcmp(buf, szFinal, iOutLen / 2), 0);
    ck_assert_str_eq(szStr, "ABF3456C890D24F");
    ck_assert_ptr_eq(p, buf + iOutLen / 2);

    DL_UINT8 puf[32];
    DL_UINT8 *ppuf = puf;
    //unpack
    printf("unpacking...\n");
    p -= (iOutLen + 1) / 2;
    _unpackNibbleRight(&p, iOutLen, &ppuf);
    phex(puf, iOutLen);
    ck_assert_uint_eq(memcmp(puf, "00000ABF3456C890D24F", iOutLen), 0);
    ck_assert_ptr_eq(ppuf, puf + iOutLen);

}
END_TEST

START_TEST(test_pack_len_ascii)
{
    DL_UINT8 buf[21];
    DL_UINT8 *p = buf;
    
    DL_UINT32 varLen = 36;
    DL_UINT32 varLenLen = 3;

    printf("pack len ascii [%d],[%d]\n", varLen, varLenLen);    
    _packLenAscii(varLen, varLenLen, &p);
    phex(buf, varLenLen);
    ck_assert_uint_eq(memcmp(buf, "036", 3), 0);
    ck_assert_ptr_eq(p, buf + varLenLen);

    DL_UINT32 olen;
    //unpack
    printf("unpacking...\n");
    p -= varLenLen;
    _unpackLenAscii(&p, varLenLen, &olen);
    printf("out len %d\n", olen);
    ck_assert_uint_eq(olen, varLen);
    ck_assert_ptr_eq(p, buf + varLenLen);
}
END_TEST

START_TEST(test_pack_len_ebcdic)
{
    DL_UINT8 buf[21];
    DL_UINT8 *p = buf;
    
    DL_UINT32 varLen = 36;
    DL_UINT32 varLenLen = 3;

    printf("pack len ebcdic [%d],[%d]\n", varLen, varLenLen);    
    _packLenEbcdic(varLen, varLenLen, &p);
    phex(buf, varLenLen);
    ck_assert_uint_eq(buf[0], A2E['0']);
    ck_assert_uint_eq(buf[1], A2E['3']);
    ck_assert_uint_eq(buf[2], A2E['6']);
    ck_assert_ptr_eq(p, buf + varLenLen);

    DL_UINT32 olen;
    //unpack
    printf("unpacking...\n");
    p -= varLenLen;
    _unpackLenEbcdic(&p, varLenLen, &olen);
    printf("out len %d\n", olen);
    ck_assert_uint_eq(olen, varLen);
    ck_assert_ptr_eq(p, buf + varLenLen);
}
END_TEST

START_TEST(test_pack_len_bcd_right)
{
    DL_UINT8 buf[21];
    DL_UINT8 *p = buf;
    
    DL_UINT32 varLen = 136;
    DL_UINT32 varLenLen = 3;

    printf("pack len bcd right [%d],[%d]\n", varLen, varLenLen);    
    _packLenBcdRight(varLen, varLenLen, &p);
    phex(buf, varLenLen);
    ck_assert_uint_eq(buf[0], 0x00);
    ck_assert_uint_eq(buf[1], 0x01);
    ck_assert_uint_eq(buf[2], 0x36);
    ck_assert_ptr_eq(p, buf + varLenLen);

    DL_UINT32 olen;
    //unpack
    printf("unpacking...\n");
    p -= varLenLen;
    _unpackLenBcdRight(&p, varLenLen, &olen);
    printf("out len %d\n", olen);
    ck_assert_uint_eq(olen, varLen);
    ck_assert_ptr_eq(p, buf + varLenLen);
}
END_TEST

START_TEST(test_pack_len_nibble_right)
{
    DL_UINT8 buf[21];
    DL_UINT8 *p = buf;
    
    DL_UINT32 varLen = 0x012F;
    DL_UINT32 varLenLen = 3;

    printf("pack len nibble right [%d],[%d]\n", varLen, varLenLen);    
    _packLenNibbleRight(varLen, varLenLen, &p);
    phex(buf, varLenLen);
    ck_assert_uint_eq(buf[0], 0x00);
    ck_assert_uint_eq(buf[1], 0x01);
    ck_assert_uint_eq(buf[2], 0x2F);
    ck_assert_ptr_eq(p, buf + varLenLen);

    DL_UINT32 olen;
    //unpack
    printf("unpacking...\n");
    p -= varLenLen;
    _unpackLenNibbleRight(&p, varLenLen, &olen);
    printf("out len %d\n", olen);
    ck_assert_uint_eq(olen, varLen);
    ck_assert_ptr_eq(p, buf + varLenLen);
}
END_TEST

START_TEST(test_pack_len_byte)
{
    DL_UINT8 buf[21];
    DL_UINT8 *p = buf;
    
    DL_UINT32 varLen = 0x012F;
    DL_UINT32 varLenLen = 3;

    printf("pack len byte [%d],[%d]\n", varLen, varLenLen);    
    _packLenByte(varLen, varLenLen, &p);
    phex(buf, varLenLen);
    ck_assert_uint_eq(buf[0], 0x00);
    ck_assert_uint_eq(buf[1], 0x01);
    ck_assert_uint_eq(buf[2], 0x2F);
    ck_assert_ptr_eq(p, buf + varLenLen);

    DL_UINT32 olen;
    //unpack
    printf("unpacking...\n");
    p -= varLenLen;
    _unpackLenByte(&p, varLenLen, &olen);
    printf("out len %d\n", olen);
    ck_assert_uint_eq(olen, varLen);
    ck_assert_ptr_eq(p, buf + varLenLen);
}
END_TEST

Suite * iso8583_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_limits;

    s = suite_create("ISO8583");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup, teardown);
    //tcase_add_test(tc_core, test_1);
    tcase_add_test(tc_core, test_pack_ascii);
    tcase_add_test(tc_core, test_pack_bcd_left);
    tcase_add_test(tc_core, test_pack_bcd_right);
    tcase_add_test(tc_core, test_pack_nibble_left);
    tcase_add_test(tc_core, test_pack_nibble_right);
    tcase_add_test(tc_core, test_pack_ebcdic);
    tcase_add_test(tc_core, test_pack_byte);


    tcase_add_test(tc_core, test_pack_len_ascii);
    //tcase_add_test(tc_core, test_pack_len_bcd_left);
    tcase_add_test(tc_core, test_pack_len_bcd_right);
    //tcase_add_test(tc_core, test_pack_len_nibble_left);
    tcase_add_test(tc_core, test_pack_len_nibble_right);
    tcase_add_test(tc_core, test_pack_len_ebcdic);
    tcase_add_test(tc_core, test_pack_len_byte);
    
    suite_add_tcase(s, tc_core);

    /* Limits test case */
    tc_limits = tcase_create("Limits");

    //tcase_add_test(tc_limits, test_2);
    /*tcase_add_test(tc_core, test_pack_ascii);
    tcase_add_test(tc_core, test_pack_bcd_left);
    tcase_add_test(tc_core, test_pack_bcd_right);
    tcase_add_test(tc_core, test_pack_nibble_left);
    tcase_add_test(tc_core, test_pack_nibble_right);
    tcase_add_test(tc_core, test_pack_ebcdic);
    tcase_add_test(tc_core, test_pack_byte);
    tcase_add_test(tc_core, test_unpack_ascii);
    tcase_add_test(tc_core, test_unpack_bcd_left);
    tcase_add_test(tc_core, test_unpack_bcd_right);
    tcase_add_test(tc_core, test_unpack_nibble_left);
    tcase_add_test(tc_core, test_unpack_nibble_right);
    tcase_add_test(tc_core, test_unpack_ebcdic);
    tcase_add_test(tc_core, test_unpack_byte);

    tcase_add_test(tc_core, test_pack_len_ascii);
    tcase_add_test(tc_core, test_pack_len_bcd_left);
    tcase_add_test(tc_core, test_pack_len_bcd_right);
    tcase_add_test(tc_core, test_pack_len_nibble_left);
    tcase_add_test(tc_core, test_pack_len_nibble_right);
    tcase_add_test(tc_core, test_pack_len_ebcdic);
    tcase_add_test(tc_core, test_pack_len_byte);
    tcase_add_test(tc_core, test_unpack_len_ascii);
    tcase_add_test(tc_core, test_unpack_len_bcd_left);
    tcase_add_test(tc_core, test_unpack_len_bcd_right);
    tcase_add_test(tc_core, test_unpack_len_nibble_left);
    tcase_add_test(tc_core, test_unpack_len_nibble_right);
    tcase_add_test(tc_core, test_unpack_len_ebcdic);
    tcase_add_test(tc_core, test_unpack_len_byte);    
    */
    suite_add_tcase(s, tc_limits);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = iso8583_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
