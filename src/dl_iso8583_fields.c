/******************************************************************************/
/*                                                                            */
/* Copyright (C) 2005-2007 Oscar Sanderson                                    */
/*                                                                            */
/* This software is provided 'as-is', without any express or implied          */
/* warranty.  In no event will the author(s) be held liable for any damages   */
/* arising from the use of this software.                                     */
/*                                                                            */
/* Permission is granted to anyone to use this software for any purpose,      */
/* including commercial applications, and to alter it and redistribute it     */
/* freely, subject to the following restrictions:                             */
/*                                                                            */
/* 1. The origin of this software must not be misrepresented; you must not    */
/*    claim that you wrote the original software. If you use this software    */
/*    in a product, an acknowledgment in the product documentation would be   */
/*    appreciated but is not required.                                        */
/*                                                                            */
/* 2. Altered source versions must be plainly marked as such, and must not be */
/*    misrepresented as being the original software.                          */
/*                                                                            */
/* 3. This notice may not be removed or altered from any source distribution. */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* ISO8583 field packers / unpackers                                          */
/*                                                                            */
/******************************************************************************/

#include "dl_iso8583_fields.h"

/******************************************************************************/
//
// for ebcdic and ascii convertiong
//
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


/******************************************************************************/
//
// FIELD HANDLER PROTOTYPES
//

DL_ERR _pack_iso_BITMAP ( DL_UINT16                    iField,
                          const DL_ISO8583_MSG        *iMsg,
                          const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
                          DL_UINT8                   **ioPtr );

DL_ERR _unpack_iso_BITMAP ( DL_UINT16                    iField,
                            DL_ISO8583_MSG              *ioMsg,
                            const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
                            DL_UINT8                   **ioPtr );

DL_ERR _pack( DL_UINT16                    iField,
              const DL_ISO8583_MSG        *iMsg,
              const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
              DL_UINT8                   **ioPtr );

DL_ERR _unpack( DL_UINT16                    iField,
                DL_ISO8583_MSG              *ioMsg,
                const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
                DL_UINT8                   **ioPtr );


DL_ERR _packLenAscii(DL_UINT32 iVarLen, DL_UINT8 iVarLenLen, DL_UINT8 **ioPtr);
DL_ERR _packAscii(DL_UINT8 *iDataPtr, DL_UINT32 iDataLen, DL_UINT32 iOutLen, DL_UINT8 **ioPtr);
DL_ERR _unpackLenAscii(DL_UINT8 **ioPtr, DL_UINT8 iVarLenLen, DL_UINT32 *oLen);
DL_ERR _unpackAscii(DL_UINT8 **ioPtr, DL_UINT32 iSize, DL_UINT8 **ioDataPtr);

DL_ERR _packLenEbcdic(DL_UINT32 iVarLen, DL_UINT8 iVarLenLen, DL_UINT8 **ioPtr);
DL_ERR _packEbcdic(DL_UINT8 *iDataPtr, DL_UINT32 iDataLen, DL_UINT32 iOutLen, DL_UINT8 **ioPtr);
DL_ERR _unpackLenEbcdic(DL_UINT8 **ioPtr, DL_UINT8 iVarLenLen, DL_UINT32 *oLen);
DL_ERR _unpackEbcdic(DL_UINT8 **ioPtr, DL_UINT32 iSize, DL_UINT8 **ioDataPtr);

DL_ERR _packLenBcdLeft(DL_UINT32 iVarLen, DL_UINT8 iVarLenLen, DL_UINT8 **ioPtr);
DL_ERR _packBcdLeft(DL_UINT8 *iDataPtr, DL_UINT32 iDataLen, DL_UINT32 iOutLen, DL_UINT8 **ioPtr);
DL_ERR _unpackLenBcdLeft(DL_UINT8 **ioPtr, DL_UINT8 iVarLenLen, DL_UINT32 *oLen);
DL_ERR _unpackBcdLeft(DL_UINT8 **ioPtr, DL_UINT32 iSize, DL_UINT8 **ioDataPtr);

DL_ERR _packLenBcdRight(DL_UINT32 iVarLen, DL_UINT8 iVarLenLen, DL_UINT8 **ioPtr);
DL_ERR _packBcdRight(DL_UINT8 *iDataPtr, DL_UINT32 iDataLen, DL_UINT32 iOutLen, DL_UINT8 **ioPtr);
DL_ERR _unpackLenBcdRight(DL_UINT8 **ioPtr, DL_UINT8 iVarLenLen, DL_UINT32 *oLen);
DL_ERR _unpackBcdRight(DL_UINT8 **ioPtr, DL_UINT32 iSize, DL_UINT8 **ioDataPtr);

DL_ERR _packLenNibbleLeft(DL_UINT32 iVarLen, DL_UINT8 iVarLenLen, DL_UINT8 **ioPtr);
DL_ERR _packNibbleLeft(DL_UINT8 *iDataPtr, DL_UINT32 iDataLen, DL_UINT32 iOutLen, DL_UINT8 **ioPtr);
DL_ERR _unpackLenNibbleLeft(DL_UINT8 **ioPtr, DL_UINT8 iVarLenLen, DL_UINT32 *oLen);
DL_ERR _unpackNibbleLeft(DL_UINT8 **ioPtr, DL_UINT32 iSize, DL_UINT8 **ioDataPtr);

DL_ERR _packLenNibbleRight(DL_UINT32 iVarLen, DL_UINT8 iVarLenLen, DL_UINT8 **ioPtr);
DL_ERR _packNibbleRight(DL_UINT8 *iDataPtr, DL_UINT32 iDataLen, DL_UINT32 iOutLen, DL_UINT8 **ioPtr);
DL_ERR _unpackLenNibbleRight(DL_UINT8 **ioPtr, DL_UINT8 iVarLenLen, DL_UINT32 *oLen);
DL_ERR _unpackNibbleRight(DL_UINT8 **ioPtr, DL_UINT32 iSize, DL_UINT8 **ioDataPtr);

DL_ERR _packLenByte(DL_UINT32 iVarLen, DL_UINT8 iVarLenLen, DL_UINT8 **ioPtr);
DL_ERR _packByte(DL_UINT8 *iDataPtr, DL_UINT32 iDataLen, DL_UINT32 iOutLen, DL_UINT8 **ioPtr);
DL_ERR _unpackLenByte(DL_UINT8 **ioPtr, DL_UINT8 iVarLenLen, DL_UINT32 *oLen);
DL_ERR _unpackByte(DL_UINT8 **ioPtr, DL_UINT32 iSize, DL_UINT8 **ioDataPtr);


/******************************************************************************/
//
// LENGTH HANDLER PROTOTYPES
//

// outputs the variable length element
// iVarLenType - e.g. kDL_ISO8583_LLVAR
static DL_ERR VarLen_Put ( DL_UINT8    iVarLenLen,
                           DL_UINT8    iVarLenType,
                           DL_UINT32   iVarLen,
                           DL_UINT32  *oLen,
                           DL_UINT8  **ioPtr );

// determines variable length element
static DL_ERR VarLen_Get ( DL_UINT8 **ioPtr,
                           DL_UINT8         iVarLenLen,
                           DL_UINT8         iVarLenType,
                           DL_UINT32        iMaxValue,
                           DL_UINT32       *oLen );


/******************************************************************************/
//
// TYPES
//

struct DL_ISO8583_TYPE_S {
    DL_ERR    (*_packLenFunc)(DL_UINT32, DL_UINT8, DL_UINT8 **);
    DL_ERR    (*_packFunc)(DL_UINT8 *, DL_UINT32, DL_UINT32, DL_UINT8 **);
    DL_ERR    (*_unpackLenFunc)(DL_UINT8 **, DL_UINT8, DL_UINT32 *);
    DL_ERR    (*_unpackFunc)(DL_UINT8 **, DL_UINT32, DL_UINT8 **);
};
typedef struct DL_ISO8583_TYPE_S DL_ISO8583_TYPE;

/******************************************************************************/
//
// VARIABLES
//
// the second column store the uncompressed field's call

#define MAKE_FIELD_TYPE_FUNC(T) {_packLen##T, _pack##T, _unpackLen##T, _unpack##T}

static DL_ISO8583_TYPE fieldTypeArr[] = {
    /* ASCII        */ MAKE_FIELD_TYPE_FUNC(Ascii),
    /* EBCDIC       */ MAKE_FIELD_TYPE_FUNC(Ebcdic),
    /* BCD_LEFT     */ MAKE_FIELD_TYPE_FUNC(BcdLeft),
    /* BCD_RIGHT    */ MAKE_FIELD_TYPE_FUNC(BcdRight),
    /* NIBBLE_LEFT  */ MAKE_FIELD_TYPE_FUNC(NibbleLeft),
    /* NIBBLE_RIGHT */ MAKE_FIELD_TYPE_FUNC(NibbleRight),
    /* BYTE         */ MAKE_FIELD_TYPE_FUNC(Byte)
    /* BMP          */ /*special handler*/ 
};

/******************************************************************************/
//
// MACROS
//

// gets the field type details
#define GetFieldType(fieldType)  (&fieldTypeArr[(fieldType)])

/******************************************************************************/

DL_ERR _DL_ISO8583_FIELD_Pack ( DL_UINT16                  iField,
                                const DL_ISO8583_MSG      *iMsg,
                                const DL_ISO8583_HANDLER  *iHandler,
                                DL_UINT8                 **ioPtr )
{
    DL_ERR                err          = kDL_ERR_NONE;
    DL_ISO8583_FIELD_DEF *fieldDefPtr  = DL_ISO8583_GetFieldDef(iField, iHandler);
    DL_UINT8             *tmpPtr        = *ioPtr;
    DL_ISO8583_MSG_FIELD *fieldPtr      = ((DL_ISO8583_MSG*)iMsg)->field + iField;
    DL_UINT32             inLen         = fieldPtr->len;     //len of the field in field unit, not include padding
    DL_UINT8             *dataPtr       = fieldPtr->ptr;
    DL_UINT32             outLen        = fieldDefPtr->len; //len of the packed field in field unit, include padding

    if (inLen > outLen) {  //too long input
        err = kDL_ERR_PACK_FLD_TOO_LONG;
    }
    
    if (!err && DL_ISO8583_IS_BITMAP(fieldDefPtr->fieldType)) {
        err = _pack_iso_BITMAP(iField, iMsg, fieldDefPtr, ioPtr);
        return err ? MAKE_FLD_ERR(iField, err) : err;
    }

    if (!err && fieldDefPtr->varLenLen > 0) {
        /* variable length handling */
        err = VarLen_Put(fieldDefPtr->varLenLen, fieldDefPtr->varLenType, inLen, &outLen, &tmpPtr);
    } 
    if (!err && DL_ISO8583_IS_PARTIAL_TYPE(fieldDefPtr->fieldType)) {  //partial byte need padding
        outLen += outLen % 2;
    }
    
    //fill the field content
    if (!err) {
        err = GetFieldType(fieldDefPtr->fieldType)->_packFunc(dataPtr, inLen, outLen, &tmpPtr);
    }
    
    *ioPtr = tmpPtr;
    
    return err ? MAKE_FLD_ERR(iField, err) : err;
}

/******************************************************************************/

DL_ERR _DL_ISO8583_FIELD_Unpack ( DL_UINT16                  iField,
                                  DL_ISO8583_MSG            *ioMsg,
                                  const DL_ISO8583_HANDLER  *iHandler,
                                  DL_UINT8                 **ioPtr )
{
    DL_ERR                err          = kDL_ERR_NONE;
    DL_ISO8583_FIELD_DEF *fieldDefPtr  = DL_ISO8583_GetFieldDef(iField, iHandler);

    DL_UINT8  *tmpPtr     = *ioPtr;
    DL_UINT32  size       = 0;
    DL_UINT32  sizeAlloc  = 0;
    DL_UINT8  *tmpDataPtr = NULL;

    if (DL_ISO8583_IS_BITMAP(fieldDefPtr->fieldType)) {
        err = _unpack_iso_BITMAP(iField, ioMsg, fieldDefPtr, ioPtr);
        return err ? MAKE_FLD_ERR(iField, err) : err;
    }
    
    /* variable length handling */
    if (!err) {
        err = VarLen_Get(&tmpPtr, fieldDefPtr->varLenLen, fieldDefPtr->varLenType, fieldDefPtr->len, &size);
    }

    /* allocate field */
    if ( !err ) {
        sizeAlloc = DL_ISO8583_IS_PARTIAL_TYPE(fieldDefPtr->fieldType) ? (size * 1) : size;
        err = _DL_ISO8583_MSG_AllocField(iField, sizeAlloc, ioMsg, &tmpDataPtr);
    }

    if ( !err ) {
        err = GetFieldType(fieldDefPtr->fieldType)->_unpackFunc(&tmpPtr, size, &tmpDataPtr);
        *tmpDataPtr = kDL_ASCII_NULL; // null terminate 
    }

    *ioPtr = tmpPtr;

    return err ? MAKE_FLD_ERR(iField, err) : err;
}

DL_ERR _pack_iso_BITMAP ( DL_UINT16                    iField,
                          const DL_ISO8583_MSG        *iMsg,
                          const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
                          DL_UINT8                   **ioPtr )
{
    DL_ERR     err         = kDL_ERR_NONE;
    DL_UINT8  *tmpPtr      = *ioPtr;
    DL_UINT16  curFieldIdx = iField;
    int        i;

    /* for each possible bitmap segment */
    for ( i=0 ; i<((kDL_ISO8583_MAX_FIELD_IDX-iField+1)+63)/64 ; i++ ) {
        DL_UINT32 ms=0, ls=0;
        int       j;

        /* move to next field */
        if(i == 0)
            curFieldIdx++;

        for ( j=0 ; j<31 + i ; j++,curFieldIdx++ ) {
            ms <<= 1;
            if ( (curFieldIdx <= kDL_ISO8583_MAX_FIELD_IDX) &&
                 (NULL != iMsg->field[curFieldIdx].ptr) )
                ms++;
        }

        for ( j=0 ; j<32 ; j++,curFieldIdx++ ) {
            ls <<= 1;
            if ( (curFieldIdx <= kDL_ISO8583_MAX_FIELD_IDX) &&
                 (NULL != iMsg->field[curFieldIdx].ptr) )
                ls++;
        }

        /* output bitmap segment (if required) */
        if ( 0 == i ) {
            /* NB 1st segment is always output */
            DL_UINT32_TO_BYTES(ms,tmpPtr);
            DL_UINT32_TO_BYTES(ls,tmpPtr+4);
            tmpPtr += 8;
        } else {
            if ( ms || ls ) {
                /* set continuation bit of previous segment */
                *(tmpPtr-8) |= 0x80;

                DL_UINT32_TO_BYTES(ms,tmpPtr);
                DL_UINT32_TO_BYTES(ls,tmpPtr+4);
                tmpPtr += 8;
            } else {
                /* no fields present, so don't output */
                break;
            }
        }
    } /* end-for(i) */

    *ioPtr = tmpPtr;

    return err;
}

/******************************************************************************/

DL_ERR _unpack_iso_BITMAP ( DL_UINT16                    iField,
                            DL_ISO8583_MSG              *ioMsg,
                            const DL_ISO8583_FIELD_DEF  *iFieldDefPtr,
                            DL_UINT8                   **ioPtr )
{
    DL_ERR     err    = kDL_ERR_NONE;
    DL_UINT8  *tmpPtr = *ioPtr;
    DL_UINT32 i = 0;

    DL_UINT16 curFieldIdx = iField;

    /* for each bitmap segment (8 bytes) */
    while(i<(((kDL_ISO8583_MAX_FIELD_IDX-iField+1)+63)/64)) {
        DL_UINT32 ms,ls;
        int       j;
        
        /* get bitmap segment (8 bytes) */
        ms = DL_BYTES_TO_UINT32(tmpPtr);
        ls = DL_BYTES_TO_UINT32(tmpPtr+4);
        tmpPtr += 8;
        
        /* move to next field */
        if(i == 0)
            curFieldIdx++;

        /* ms part */
        for ( j=30 + i ; j>=0 ; j--,curFieldIdx++ ) {
            if ( DL_BIT_TEST(ms,j) ) {
                if ( curFieldIdx > kDL_ISO8583_MAX_FIELD_IDX )
                    return kDL_ERR_UNPACK_BMP;

                /* set length to non-zero value to indicate field presence */
                ioMsg->field[curFieldIdx].len = 1;
            }
        } /* end-for(j) */

        /* ls part */
        for ( j=31 ; j>=0 ; j--,curFieldIdx++ ) {
            if ( DL_BIT_TEST(ls,j) ) {
                if ( curFieldIdx > kDL_ISO8583_MAX_FIELD_IDX )
                    return kDL_ERR_UNPACK_BMP;
                
                /* set length to non-zero value to indicate field presence */
                ioMsg->field[curFieldIdx].len = 1;
            }
        } /* end-for(j) */
        
        /* stop if no more bitmap segments */
        if( i == 0) {
            if ( 0 == DL_BIT_TEST(ms,31) )
                break;
        }
        i++;
    }


    *ioPtr = tmpPtr;

    return err;
}


/******************************************************************************/

// returns the bcd encoded value - based on decValue (0..99)
#define output_bcd_byte(decValue) ((DL_UINT8)((((decValue)/10)<<4)|((decValue)%10)))

// outputs the variable length element
// iVarLenType - e.g. kDL_ISO8583_LLVAR
static DL_ERR VarLen_Put ( DL_UINT8                     iVarLenLen,
                           DL_UINT8                     iVarLenType,
                           DL_UINT32                    iVarLen,
                           DL_UINT32                   *oLen,
                           DL_UINT8                   **ioPtr )
{
    DL_ERR       err    = kDL_ERR_NONE;
    DL_UINT8    *tmpPtr = *ioPtr;

    // fixed len, we calculate the olen
    if (iVarLenLen <= 0) {
        return err;
    }

    err = GetFieldType(iVarLenType)->_packLenFunc(iVarLen, iVarLenLen, &tmpPtr);
    *oLen = iVarLen;
    *ioPtr = tmpPtr;

    return err;
}

/******************************************************************************/

// determines variable length element
static DL_ERR VarLen_Get ( DL_UINT8       **ioPtr,
                           DL_UINT8         iVarLenLen,
                           DL_UINT8         iVarLenType,
                           DL_UINT32        iMaxValue,
                           DL_UINT32       *oLen )
{
    DL_ERR    err    = kDL_ERR_NONE;
    DL_UINT8 *tmpPtr = (DL_UINT8*)*ioPtr;

    /* init outputs */
    *oLen = iMaxValue;

    if (iVarLenLen <= 0) {
        return err;
    }
    
    if ( !err ) {
        err = GetFieldType(iVarLenType)->_unpackLenFunc(&tmpPtr, iVarLenLen, oLen);
        /* limit if exceeds max */
        *oLen = MIN(iMaxValue, *oLen);
    }

    *ioPtr = tmpPtr;

    return err;
}


//parse ascii
DL_ERR _packLenAscii(DL_UINT32 iVarLen, DL_UINT8 iVarLenLen, DL_UINT8 **ioPtr)
{
    DL_ERR    err = kDL_ERR_NONE;
    DL_UINT32 iMod = 0;
    DL_UINT8  *tmpPtr = *ioPtr;
    DL_UINT8 i = 0;

    if (iVarLenLen <= 0) {
        return kDL_ERR_PACK_ASCII_LEN;
    }
    
    for (i = iVarLenLen; i > 0; i--, iVarLen /= 10) {
        tmpPtr[i - 1] = '0' + iVarLen % 10;
    }

    tmpPtr += iVarLenLen;

    *ioPtr = tmpPtr;
    return err;
}

DL_ERR _packAscii(DL_UINT8 *iDataPtr, DL_UINT32 iDataLen, DL_UINT32 iOutLen, DL_UINT8 **ioPtr)
{
    DL_ERR err = kDL_ERR_NONE;
    
    if (iDataLen > iOutLen) {
        err = kDL_ERR_PACK_ASCII;
    } else {
        memcpy(*ioPtr, iDataPtr, iDataLen);
        if (iDataLen < iOutLen) {
            memset(*ioPtr + iDataLen, (int)kDL_ASCII_SP, iOutLen - iDataLen);
        }
        *ioPtr += iOutLen;
    }
    return err;
}

DL_ERR _unpackLenAscii(DL_UINT8 **ioPtr, DL_UINT8 iVarLenLen, DL_UINT32 *oLen)
{
    DL_ERR err = kDL_ERR_NONE;
    DL_UINT8 i = 0;
    DL_UINT32 len = 0;
    
    for (i = 0; i < iVarLenLen; i++) {
        len = len * 10 + *(*ioPtr)++ - '0';
    }

    *oLen = len;
    
    return err;    
}

DL_ERR _unpackAscii(DL_UINT8 **ioPtr, DL_UINT32 iSize, DL_UINT8 **ioDataPtr)
{
    DL_ERR err = kDL_ERR_NONE;

    memcpy(*ioDataPtr, *ioPtr, iSize);

    *ioDataPtr += iSize;
    *ioPtr += iSize;
    return err;
}

//parse ebcdic
DL_ERR _packLenEbcdic(DL_UINT32 iVarLen, DL_UINT8 iVarLenLen, DL_UINT8 **ioPtr)
{
    DL_ERR    err = kDL_ERR_NONE;
    DL_UINT32 iMod = 0;
    DL_UINT8  *tmpPtr = *ioPtr;
    DL_UINT8 i = 0;

    for (i = iVarLenLen; i > 0; i--, iVarLen /= 10) {
        tmpPtr[i - 1] = A2E['0' + iVarLen % 10];
    }

    tmpPtr += iVarLenLen;

    *ioPtr = tmpPtr;
    return err;    
}

DL_ERR _packEbcdic(DL_UINT8 *iDataPtr, DL_UINT32 iDataLen, DL_UINT32 iOutLen, DL_UINT8 **ioPtr)
{
    DL_ERR err = kDL_ERR_NONE;
    DL_UINT32 i = 0;
    
    if (iDataLen > iOutLen) {
        err = kDL_ERR_PACK_EBCDIC;
    } else {
        memcpy(*ioPtr, iDataPtr, iDataLen);
        if (iDataLen < iOutLen) {
            memset(*ioPtr + iDataLen, (int)kDL_ASCII_SP, iOutLen - iDataLen);
        }

        //ascii to ebcdic
        for (i = 0; i < iOutLen; i++) {
            *(*ioPtr + i) = A2E[*(*ioPtr + i)];
        }
        
        *ioPtr += iOutLen;
    }
    return err;

}

DL_ERR _unpackLenEbcdic(DL_UINT8 **ioPtr, DL_UINT8 iVarLenLen, DL_UINT32 *oLen)
{
    DL_ERR err = kDL_ERR_NONE;
    DL_UINT8 i = 0;
    DL_UINT32 len = 0;
    
    for (i = 0; i < iVarLenLen; i++) {
        len = len * 10 + E2A[*(*ioPtr)++] - '0';
    }

    *oLen = len;
    return err;    
}

DL_ERR _unpackEbcdic(DL_UINT8 **ioPtr, DL_UINT32 iSize, DL_UINT8 **ioDataPtr)
{
    DL_ERR err = kDL_ERR_NONE;
    DL_UINT32 i = 0;
    
    memcpy(*ioDataPtr, *ioPtr, iSize);

    for (i = 0; i < iSize; i++) {
        *(*ioDataPtr + i) = E2A[*(*ioDataPtr + i)];
    }
    
    *ioDataPtr += iSize;
    *ioPtr += iSize;
    return err;
}

DL_UINT32 numOfDigits(DL_UINT32 iNum)
{
    DL_UINT32 i = 1;   //at least one digit

    while ((iNum /= 10) > 0)
        i++;
    
    return i;
}

//parse bcd left align
DL_ERR _packLenBcdLeft(DL_UINT32 iVarLen, DL_UINT8 iVarLenLen, DL_UINT8 **ioPtr)
{
    DL_ERR err = kDL_ERR_NONE;
    /*
      DL_UINT32 i = 0;
      DL_UINT32 iDigitNum = numOfDigits(iVarLen);
      DL_UINT32 iTmp = iVarLen;
      DL_UINT8 *tmpPtr = *ioPtr + (iDigitNum - 1) / 2;  // point to last bcd's byte
    
      if (iDigitNum % 2) {  //odd num of digits
      *tmpPtr-- = ((iTmp % 10) << 4);
      iTmp /= 10;
      }
      while (iTmp > 0) {
      *tmpPtr-- = output_bcd_byte(iTmp % 100);
      iTmp /= 100;
      }
    
      *ioPtr += iVarLenLen;
      */
    //we do not support length in bcd left
    err = kDL_ERR_OTHER;
    return err;
}

DL_ERR _packBcdLeft(DL_UINT8 *iDataPtr, DL_UINT32 iDataLen, DL_UINT32 iOutLen, DL_UINT8 **ioPtr)
{
    DL_ERR err = kDL_ERR_NONE;
    DL_UINT8 *tmpPtr = *ioPtr;
    DL_UINT8 i = 0;

    //check out space is double nibbles
    if (iOutLen % 2) {
        return kDL_ERR_PACK_BCDL;
    }

    //init and set padding
    memset(tmpPtr, 0, iOutLen / 2);
    
    for (i = 0; i < iDataLen; i++) {
        if (i % 2) {
            *tmpPtr++ += (*iDataPtr++ - '0') & 0x0F ;
        } else {
            *tmpPtr += ((*iDataPtr++ - '0') << 4) & 0xF0;
        }
    }
    
    *ioPtr += iOutLen / 2;
    return err;
}

DL_ERR _unpackLenBcdLeft(DL_UINT8 **ioPtr, DL_UINT8 iVarLenLen, DL_UINT32 *oLen)
{
    DL_ERR err = kDL_ERR_NONE;

    //we do not support length in bcd left align
    err = kDL_ERR_OTHER;
    
    return err;
}

DL_ERR _unpackBcdLeft(DL_UINT8 **ioPtr, DL_UINT32 iSize, DL_UINT8 **ioDataPtr)
{
    DL_ERR err = kDL_ERR_NONE;
    DL_UINT32 i = 0;
    DL_UINT8 *tmpPtr = *ioPtr;
    DL_UINT8 *tmpDataPtr = *ioDataPtr;
    
    for (i = 0; i < iSize; i++) {
        if (i % 2) {
            *tmpDataPtr++ = (*tmpPtr++ & 0x0F) + '0';
        } else {
            *tmpDataPtr++ = ((*tmpPtr >> 4) & 0x0F) + '0';
        }
    }

    if (iSize % 2) {  //skip last bcd
        tmpPtr++;
    }
            
    *ioPtr = tmpPtr;
    *ioDataPtr = tmpDataPtr;
    return err;
}


//parse bcd right right
DL_ERR _packLenBcdRight(DL_UINT32 iVarLen, DL_UINT8 iVarLenLen, DL_UINT8 **ioPtr)
{
    DL_ERR err = kDL_ERR_NONE;

    DL_UINT32 i = 0;
    DL_UINT32 iDigitNum = numOfDigits(iVarLen);
    DL_UINT32 lenTmp = iVarLen;
    DL_UINT8 *tmpPtr = *ioPtr + iVarLenLen - 1;  // point to last bcd's byte

    memset(*ioPtr, 0, iVarLenLen);
    
    for (i = 0; i < iDigitNum; i++, lenTmp /= 10) {
        if (i % 2) {
            *tmpPtr-- += ((lenTmp % 10) << 4);
        } else {
            *tmpPtr += lenTmp % 10;
        }        
    }
        
    *ioPtr += iVarLenLen;

    return err;
}

DL_ERR _packBcdRight(DL_UINT8 *iDataPtr, DL_UINT32 iDataLen, DL_UINT32 iOutLen, DL_UINT8 **ioPtr)
{
    DL_ERR err = kDL_ERR_NONE;
    DL_UINT8 *tmpPtr = *ioPtr + iOutLen / 2 - 1 ;
    DL_UINT8 *iDataTmpPtr = iDataPtr + iDataLen - 1;
    DL_UINT8 i = 0;

    //check out space is double nibbles
    if (iOutLen % 2) {
        return kDL_ERR_PACK_BCDR;
    }

    //init and set padding
    memset(*ioPtr, 0, iOutLen / 2);
    
    for (i = 0; i < iDataLen; i++) {
        if (i % 2) {
            *tmpPtr-- += ((*iDataTmpPtr-- - '0') << 4) & 0xF0;
        } else {
            *tmpPtr += (*iDataTmpPtr-- - '0') & 0x0F;
        }
    }
    
    *ioPtr += iOutLen / 2;
    return err;
}

DL_ERR _unpackLenBcdRight(DL_UINT8 **ioPtr, DL_UINT8 iVarLenLen, DL_UINT32 *oLen)
{
    DL_ERR err = kDL_ERR_NONE;
    DL_UINT8 *tmpPtr = *ioPtr;
    DL_UINT32 i = 0;
    DL_UINT32 lenTmp = 0;
    
    for (i = 0; i < iVarLenLen; i++) {
        lenTmp *= 10;
        lenTmp += ((*tmpPtr >> 4) & 0x0F);
        lenTmp *= 10;
        lenTmp += (*tmpPtr++ & 0x0F);
    }
    *ioPtr = tmpPtr;
    *oLen = lenTmp;
    return err;
}

DL_ERR _unpackBcdRight(DL_UINT8 **ioPtr, DL_UINT32 iSize, DL_UINT8 **ioDataPtr)
{
    DL_ERR err = kDL_ERR_NONE;
    DL_UINT8 *tmpPtr = *ioPtr;
    DL_UINT8 *tmpDataPtr = *ioDataPtr;
    DL_UINT32 i = 0;
    
    for (i = 0; i < iSize; i++) {
        if (i % 2) {
            tmpDataPtr[i] = (tmpPtr[i / 2] & 0x0F) + '0';
        } else {
            tmpDataPtr[i] = ((tmpPtr[i / 2] >> 4) & 0x0F) + '0';
        }
    }

    *ioPtr += (iSize + 1) / 2;
    *ioDataPtr += iSize;
    return err;
}


//parse nibble left
DL_ERR _packLenNibbleLeft(DL_UINT32 iVarLen, DL_UINT8 iVarLenLen, DL_UINT8 **ioPtr)
{
    DL_ERR err = kDL_ERR_NONE;

    //we do not support length of nibble left align
    err= kDL_ERR_OTHER;
    
    return err;
}

DL_ERR _packNibbleLeft(DL_UINT8 *iDataPtr, DL_UINT32 iDataLen, DL_UINT32 iOutLen, DL_UINT8 **ioPtr)
{
    DL_ERR err = kDL_ERR_NONE;
    DL_UINT8 *tmpPtr = *ioPtr;
    DL_UINT8 i = 0;
    DL_UINT8 tmpChar = 0;
    
    //check out space is double nibbles
    if (iOutLen % 2) {
        return kDL_ERR_PACK_NIBBLEL;
    }

    //padding
    memset(tmpPtr, 0, iOutLen / 2);
    
    for (i = 0; i < iDataLen; i++) {
        if (*iDataPtr >= 'A')
            tmpChar = *iDataPtr - 'A' + 10;
        else
            tmpChar = *iDataPtr - '0';
        
        if (i % 2) {
            *tmpPtr++ += tmpChar & 0x0F;
        } else {
            *tmpPtr += (tmpChar << 4) & 0xF0;
        }
        iDataPtr++;
    }
    
    *ioPtr += iOutLen / 2;
    return err;
}

DL_ERR _unpackLenNibbleLeft(DL_UINT8 **ioPtr, DL_UINT8 iVarLenLen, DL_UINT32 *oLen)
{
    DL_ERR err = kDL_ERR_NONE;
    //we do not support length of nibble left align
    err = kDL_ERR_OTHER;
    return err;
}

DL_ERR _unpackNibbleLeft(DL_UINT8 **ioPtr, DL_UINT32 iSize, DL_UINT8 **ioDataPtr)
{
    DL_ERR err = kDL_ERR_NONE;
    DL_UINT32 i = 0;
    DL_UINT8 *tmpPtr = *ioPtr;
    DL_UINT8 *tmpDataPtr = *ioDataPtr;
    DL_UINT8 tmpN = 0;
    
    for (i = 0; i < iSize; i++) {
        if (i % 2) {
            tmpN = (*tmpPtr++ & 0x0F);
        } else {
            tmpN = (*tmpPtr >> 4) & 0x0F;
        }
        if (tmpN >= 0x0A) {
            *tmpDataPtr++ = tmpN - 0x0A + 'A';
        } else {
            *tmpDataPtr++ = tmpN + '0';
        }
    }
    
    *ioPtr = tmpPtr;
    *ioDataPtr = tmpDataPtr;
    return err;
}


DL_ERR _packLenNibbleRight(DL_UINT32 iVarLen, DL_UINT8 iVarLenLen, DL_UINT8 **ioPtr)
{
    DL_ERR err = kDL_ERR_NONE;
    DL_UINT8 *tmpPtr = *ioPtr + iVarLenLen - 1;

    //init and set padding
    memset(*ioPtr, 0, iVarLenLen);
    
    *ioPtr += iVarLenLen;
    
    while (iVarLenLen-- > 0) {
        *tmpPtr-- = iVarLen & 0xFF;
        iVarLen >>= 8;
    }

    return err;
}

DL_ERR _packNibbleRight(DL_UINT8 *iDataPtr, DL_UINT32 iDataLen, DL_UINT32 iOutLen, DL_UINT8 **ioPtr)
{
    DL_ERR err = kDL_ERR_NONE;
    DL_UINT8 *tmpPtr = *ioPtr + iOutLen / 2 - 1;
    DL_UINT8 *iDataTmpPtr = iDataPtr + iDataLen - 1;
    DL_UINT8 i = 0;
    DL_UINT8 tmpN = 0;
    
    //check out space is double nibbles
    if (iOutLen % 2) {
        return kDL_ERR_PACK_NIBBLER;
    }

    //padding
    memset(*ioPtr, 0, iOutLen / 2);
    
    for (i = 0; i < iDataLen; i++, iDataTmpPtr--) {
        if (*iDataTmpPtr >= 'A')
            tmpN = *iDataTmpPtr - 'A' + 10;
        else
            tmpN = *iDataTmpPtr - '0';

        if (i % 2) {
            *tmpPtr-- += (tmpN << 4) & 0xF0 ;
        } else {
            *tmpPtr += tmpN & 0x0F;
        }
    }
    
    *ioPtr += iOutLen / 2;
    return err;
}

DL_ERR _unpackLenNibbleRight(DL_UINT8 **ioPtr, DL_UINT8 iVarLenLen, DL_UINT32 *oLen)
{
    DL_ERR err = kDL_ERR_NONE;
    DL_UINT32 tmpLen = 0;
    DL_UINT8 *tmpPtr = *ioPtr;
    DL_UINT32 i = 0;
    
    if (iVarLenLen > 4) {
        return kDL_ERR_UNPACK_NIBBLE_LEN;
    }

    *ioPtr += iVarLenLen;
    
    while (iVarLenLen > 0) {
        tmpLen += tmpPtr[iVarLenLen - 1] << i;
        i += 8;
        iVarLenLen--;
    }
    
    *oLen = tmpLen;
    return err;
}

DL_ERR _unpackNibbleRight(DL_UINT8 **ioPtr, DL_UINT32 iSize, DL_UINT8 **ioDataPtr)
{
    DL_ERR err = kDL_ERR_NONE;
    DL_UINT32 i = 0;
    DL_UINT8 *tmpPtr = *ioPtr + (iSize - 1) / 2;
    DL_UINT8 *tmpDataPtr = *ioDataPtr + iSize - 1;
    DL_UINT8 tmpN = 0;
    
    for (i = 0; i < iSize; i++) {
        if (i % 2) {
            tmpN = (*tmpPtr-- >> 4) & 0x0F;
        } else {
            tmpN = *tmpPtr & 0x0F;
        }
        if (tmpN >= 0x0A) {
            *tmpDataPtr-- = tmpN - 0x0A + 'A';
        } else {
            *tmpDataPtr-- = tmpN + '0';
        }
    }
    
    *ioPtr += (iSize + 1) / 2;
    *ioDataPtr += iSize;
    return err;
}

DL_ERR _packLenByte(DL_UINT32 iVarLen, DL_UINT8 iVarLenLen, DL_UINT8 **ioPtr)
{
    DL_ERR err = kDL_ERR_NONE;
    err = _packLenNibbleRight(iVarLen, iVarLenLen, ioPtr);
    return err;
}

DL_ERR _packByte(DL_UINT8 *iDataPtr, DL_UINT32 iDataLen, DL_UINT32 iOutLen, DL_UINT8 **ioPtr)
{
    DL_ERR err = kDL_ERR_NONE;
    
    if (iDataLen > iOutLen) {
        err = kDL_ERR_PACK_BYTE;
    } else {
        memcpy(*ioPtr, iDataPtr, iDataLen);
        if (iDataLen < iOutLen) {
            memset(*ioPtr + iDataLen, (int)0x00, iOutLen - iDataLen);
        }
        *ioPtr += iOutLen;
    }
    return err;
}

DL_ERR _unpackLenByte(DL_UINT8 **ioPtr, DL_UINT8 iVarLenLen, DL_UINT32 *oLen)
{
    DL_ERR err = kDL_ERR_NONE;
    err = _unpackLenNibbleRight(ioPtr, iVarLenLen, oLen);
    return err;
}

DL_ERR _unpackByte(DL_UINT8 **ioPtr, DL_UINT32 iSize, DL_UINT8 **ioDataPtr)
{
    DL_ERR err = kDL_ERR_NONE;

    memcpy(*ioDataPtr, *ioPtr, iSize);

    *ioDataPtr += iSize;
    *ioPtr += iSize;
    return err;
}

