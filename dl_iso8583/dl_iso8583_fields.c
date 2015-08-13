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


DL_ERR _packLenAscii();
DL_ERR _packAscii();
DL_ERR _unpackLenAscii();
DL_ERR _unpackAscii();



/******************************************************************************/
//
// LENGTH HANDLER PROTOTYPES
//

// outputs the variable length element
// iVarLenType - e.g. kDL_ISO8583_LLVAR
static DL_ERR VarLen_Put ( DL_UINT8    iVarLenType,
                           DL_UINT32   iActLen,
                           DL_UINT32  *ioReqLen,
                           DL_UINT8  **ioPtr );
static DL_ERR VarLen_Put_ex ( DL_UINT8    iVarLenType,
                              DL_UINT32   iActLen,
                              DL_UINT32  *ioReqLen,
                              DL_UINT8  **ioPtr );


// determines variable length element
static DL_ERR VarLen_Get ( const DL_UINT8 **ioPtr,
                           DL_UINT8         iVarLenDigits,
                           DL_UINT16        iMaxValue,
                           DL_UINT16       *oLen );
static DL_ERR VarLen_Get_ex ( const DL_UINT8 **ioPtr,
                              DL_UINT8         iVarLenDigits,
                              DL_UINT16        iMaxValue,
                              DL_UINT16       *oLen );


/******************************************************************************/
//
// TYPES
//

struct DL_ISO8583_TYPE_S {
    DL_ERR    (*_packLenFunc)(DL_UINT32, DL_UINT8, DL_UINT8**);
    DL_ERR    (*_packFunc)(DL_UINT8*, DL_UINT32, DL_UINT32, DL_UINT8**);
    DL_ERR    (*_unpackLenFunc)(DL_UINT8**, DL_UINT8, DL_UINT32*);
    DL_ERR    (*_unpackFunc)(DL_UINT8**, DL_UINT32, DL_UINT8**);
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
    /* BYTE         */ MAKE_FIELD_TYPE_FUNC(Byte),
    /* BMP          */ MAKE_FIELD_TYPE_FUNC(Bmp)
};

/******************************************************************************/
//
// MACROS
//

// gets the field type details
#define GetFieldType(fieldType)  (&fieldTypeArr[(fieldType))

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
    DL_UINT32             outLen        = iFieldDefPtr->len; //len of the packed field in field unit, include padding
    
    /* variable length handling */
    err = VarLen_Put(fieldDefPtr->varLenLen, fieldDefPtr->varLenType, inLen, outLen, &tmpPtr);

    //fill the field content
    if (!err) {
        err = GetFieldType(fieldDefPtr->fieldType)->_packFunc(dataPtr, inLen, outLen, &tmpPtr);
    }
    
    *ioPtr = tmpPtr;
    
    return err;
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
    
    /* variable length handling */
    err = VarLen_Get(&tmpPtr, fieldDefPtr->varLenLen, fieldDefPtr->varLenType, fieldDefPtr->len, &size);

    /* allocate field */
    if ( !err ) {
        sizeAlloc = DL_ISO8583_FIELD_LEN_IN_BYTE(fieldDefPtr->fieldType, size);
        err = _DL_ISO8583_MSG_AllocField(iField, sizeAlloc, ioMsg, &tmpDataPtr);
    }

    if ( !err ) {
        err = GetFieldType(fieldDefPtr->fieldType)->_unpackFunc(&tmpPtr, size, &tmpDataPtr);
        *tmpDataPtr = kDL_ASCII_NULL; // null terminate 
    }

    *ioPtr = tmpPtr;

    return err;    
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
    for ( i=0 ; i<((kDL_ISO8583_MAX_FIELD_IDX-iField+1)+63)/64 ; i++ )
    {
        DL_UINT32 ms=0,
                ls=0;
        int       j;

        /* move to next field */
        if(i == 0)
            curFieldIdx++;

        for ( j=0 ; j<31 + i ; j++,curFieldIdx++ )
        {
            ms <<= 1;
            if ( (curFieldIdx <= kDL_ISO8583_MAX_FIELD_IDX) &&
                 (NULL != iMsg->field[curFieldIdx].ptr) )
                ms++;
        }

        for ( j=0 ; j<32 ; j++,curFieldIdx++ )
        {
            ls <<= 1;
            if ( (curFieldIdx <= kDL_ISO8583_MAX_FIELD_IDX) &&
                 (NULL != iMsg->field[curFieldIdx].ptr) )
                ls++;
        }

        /* output bitmap segment (if required) */
        if ( 0 == i )
        {
            /* NB 1st segment is always output */
            DL_UINT32_TO_BYTES(ms,tmpPtr);
            DL_UINT32_TO_BYTES(ls,tmpPtr+4);
            tmpPtr += 8;
        }
        else
        {
            if ( ms || ls )
            {
                /* set continuation bit of previous segment */
                *(tmpPtr-8) |= 0x80;

                DL_UINT32_TO_BYTES(ms,tmpPtr);
                DL_UINT32_TO_BYTES(ls,tmpPtr+4);
                tmpPtr += 8;
            }
            else
            {
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

    {
        DL_UINT16 curFieldIdx = iField;

        /* for each bitmap segment (8 bytes) */
        while(i<(((kDL_ISO8583_MAX_FIELD_IDX-iField+1)+63)/64))
        {
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
            for ( j=30 + i ; j>=0 ; j--,curFieldIdx++ )
            {
                if ( DL_BIT_TEST(ms,j) )
                {
                    if ( curFieldIdx > kDL_ISO8583_MAX_FIELD_IDX )
                        return kDL_ERR_OTHER;

                    /* set length to non-zero value to indicate field presence */
                    ioMsg->field[curFieldIdx].len = 1;
                }
            } /* end-for(j) */

            /* ls part */
            for ( j=31 ; j>=0 ; j--,curFieldIdx++ )
            {
                if ( DL_BIT_TEST(ls,j) )
                {
                    if ( curFieldIdx > kDL_ISO8583_MAX_FIELD_IDX )
                        return kDL_ERR_OTHER;

                    /* set length to non-zero value to indicate field presence */
                    ioMsg->field[curFieldIdx].len = 1;
                }
            } /* end-for(j) */

            /* stop if no more bitmap segments */
            if( i == 0)
            {
                if ( 0 == DL_BIT_TEST(ms,31) )
                    break;
            }
            i++;
        }
    }

    *ioPtr = tmpPtr;

    return err;
}


/******************************************************************************/

// returns the bcd encoded value - based on decValue (0..99)
#define output_bcd_byte(decValue) ((DL_UINT8)((((decValue)/10)<<4)|((decValue)%10)))

// outputs the variable length element
// iVarLenType - e.g. kDL_ISO8583_LLVAR
static DL_ERR VarLen_Put ( DL_UINT8                     varLenLen,
                           DL_UINT8                     varLenType,
                           DL_UINT32                    inLen,
                           DL_UINT32                   *outLen,
                           DL_UINT8                   **ioPtr )
{
    DL_ERR       err    = kDL_ERR_NONE;
    DL_UINT8    *tmpPtr = *ioPtr;

    // fixed len, we do nothing
    if (varLenLen <= 0) {
        return err;
    }

    err = GetFieldType(fieldDefPtr->varLenType)->_packLenFunc(inLen, varLenLen, &tmpPtr);

    *ioPtr = tmpPtr;

    return err;
}

/******************************************************************************/

// determines variable length element
static DL_ERR VarLen_Get ( const DL_UINT8 **ioPtr,
                           DL_UINT8         varLenLen,
                           DL_UINT8         varLenType,
                           DL_UINT32        iMaxValue,
                           DL_UINT32       *oLen )
{
    DL_ERR    err    = kDL_ERR_NONE;
    DL_UINT8 *tmpPtr = (DL_UINT8*)*ioPtr;

    /* init outputs */
    *oLen = iMaxValue;

    if (varLenLen <= 0) {
        return err;
    }
    
    if ( !err ) {
        err = GetFieldType(fieldDefPtr->varLenType)->_unpackLenFunc(&tmpPtr, varLenLen, oLen);
        /* limit if exceeds max */
        *oLen = MIN(iMaxValue, *oLen);
    }

    *ioPtr = tmpPtr;

    return err;
}

