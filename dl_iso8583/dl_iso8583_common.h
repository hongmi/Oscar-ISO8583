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

#ifndef __INC_DL_ISO8583_COMMON
#define __INC_DL_ISO8583_COMMON

#include "dl_base.h"
#include "dl_err.h"
#include "dl_mem.h"

/******************************************************************************/
//
// CONSTANTS
//

#define kDL_ISO8583_MAX_FIELD_IDX               128

// NB special type for bitmap continuation bit
#define kDL_ISO8583_CONTVAR             0

/****************************************************************************/
//
// CONSTANTS (field type)
//
#define kDL_ISO8583_ASCII            0
#define kDL_ISO8583_EBCDIC           1
#define kDL_ISO8583_BCD_LEFT         2
#define kDL_ISO8583_BCD_RIGHT        3
#define kDL_ISO8583_NIBBLE_LEFT      4
#define kDL_ISO8583_NIBBLE_RIGHT     5
#define kDL_ISO8583_BYTE             6
#define kDL_ISO8583_BMP              7

/******************************************************************************/
//
// MACROS
//

// Indicates whether the field type is BITMAP
#define DL_ISO8583_IS_BITMAP(fieldType)  (kDL_ISO8583_BMP==(fieldType))

// Gets the field definition for the specified handler
#define DL_ISO8583_GetFieldDef(fieldIdx,handler) ((DL_ISO8583_FIELD_DEF*)(((handler)->fieldArr)+(fieldIdx)))

// Indicates whehter the field type is valid
#define DL_ISO8583_INVALID_FIELD_TYPE(t) (((t) >= kDL_ISO8583_ASCII) && ((t) <= kDL_ISO8583_BMP))

// bcd, nibble are partial byte
#define DL_ISO8583_IS_PARTIAL_TYPE(fieldType) \
    ((fieldType) == kDL_ISO8583_BCD_LEFT      ||     \
     (fieldType) == kDL_ISO8583_BCD_RIGHT     ||     \
     (fieldType) == kDL_ISO8583_NIBBLE_LEFT   ||     \
     (fieldType) == kDL_ISO8583_NIBBLE_RIGHT)

/******************************************************************************/
//
// TYPES
//

struct DL_ISO8583_FIELD_DEF_S {
    DL_UINT8   varLenType;     /* the varlen's type */
    DL_UINT8   varLenLen;      /* number of varlen bytes in byte */
    DL_UINT32  len;            /* length for fixed size / max-len for variables */
    DL_UINT8   fieldType;      /* fied type, determine the unit of field's len counting.*/
};
typedef struct DL_ISO8583_FIELD_DEF_S DL_ISO8583_FIELD_DEF;

struct DL_ISO8583_HANDLER_S {
    DL_ISO8583_FIELD_DEF *fieldArr;
    DL_UINT8              fieldItems;
};
typedef struct DL_ISO8583_HANDLER_S DL_ISO8583_HANDLER;

struct DL_ISO8583_MSG_FIELD_S
{
    DL_UINT16  len; /* used mainly for binary fields               */
    DL_UINT8  *ptr; /* null terminated data bytes (even if binary) */
};
typedef struct DL_ISO8583_MSG_FIELD_S DL_ISO8583_MSG_FIELD;

struct DL_ISO8583_MSG_S {
    /* static memory details */
    DL_UINT8 *sPtrNext; /* next static point - NULL if dynamic mode  */
    DL_UINT8 *sPtrEnd;  /* end of the static buffer (if static mode) */

    /* NB bitmap is not stored, it is implied if the field is set */

    /* fields */
    DL_ISO8583_MSG_FIELD field[1+kDL_ISO8583_MAX_FIELD_IDX];
};
typedef struct DL_ISO8583_MSG_S DL_ISO8583_MSG;

/******************************************************************************/

// attempts to allocate memory for a field within the ISO8583 message
// NB used internally - not for client usage
// returns: error code
DL_ERR _DL_ISO8583_MSG_AllocField ( DL_UINT16        iField,
                                    DL_UINT16        iSize,
                                    DL_ISO8583_MSG  *ioMsg,
                                    DL_UINT8       **oPtr );

/******************************************************************************/

// sets the handler details in 'oData'
// returns: none
void DL_ISO8583_COMMON_SetHandler ( DL_ISO8583_FIELD_DEF *iFieldDefArr,
                                    DL_UINT8              iFieldDefItems,
                                    DL_ISO8583_HANDLER   *oData );

/******************************************************************************/

#endif /* __INC_DL_ISO8583_COMMON */
