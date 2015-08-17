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
/* dl_iso8583_defs_1993.c - ISO8583 1993 Definitions                          */
/*                                                                            */
/******************************************************************************/

#include "dl_iso8583_defs_1993.h"

/******************************************************************************/

static DL_ISO8583_FIELD_DEF _iso8583_1993_fields[] = {
    /*   0 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    4, kDL_ISO8583_BCD_LEFT  }, // Message Type Indicator
    /*   1 */ {kDL_ISO8583_BCD_RIGHT  ,      0,   16, kDL_ISO8583_BMP       }, // Bitmap
    /*   2 */ {kDL_ISO8583_BCD_RIGHT  ,      1,   19, kDL_ISO8583_BCD_LEFT  }, // Primary Account Number
    /*   3 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    6, kDL_ISO8583_BCD_LEFT  }, // Processing Code
    /*   4 */ {kDL_ISO8583_BCD_RIGHT  ,      0,   12, kDL_ISO8583_BCD_RIGHT }, // Amount, Txn
    /*   5 */ {kDL_ISO8583_BCD_RIGHT  ,      0,   12, kDL_ISO8583_BCD_RIGHT }, // Amount, Reconciliation
    /*   6 */ {kDL_ISO8583_BCD_RIGHT  ,      0,   12, kDL_ISO8583_BCD_RIGHT }, // Amount, Cardholder Billing
    /*   7 */ {kDL_ISO8583_BCD_RIGHT  ,      0,   10, kDL_ISO8583_BCD_LEFT  }, // Date and Time, Transmission
    /*   8 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    8, kDL_ISO8583_BCD_RIGHT  }, // Amount, Cardholder Billing Fee
    /*   9 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    8, kDL_ISO8583_BCD_RIGHT  }, // Conversion Rate, Reconciliation
    /*  10 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    8, kDL_ISO8583_BCD_RIGHT  }, // Conversion Rate, Cardholder Billing
    /*  11 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    6, kDL_ISO8583_BCD_RIGHT  }, // Systems Trace Audit Number
    /*  12 */ {kDL_ISO8583_BCD_RIGHT  ,      0,   12, kDL_ISO8583_BCD_RIGHT  }, // Date and Time, Local Txn
    /*  13 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    4, kDL_ISO8583_BCD_RIGHT  }, // Date, Effective
    /*  14 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    4, kDL_ISO8583_BCD_RIGHT  }, // Date, Expiration
    /*  15 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    6, kDL_ISO8583_BCD_RIGHT  }, // Date, Settlement
    /*  16 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    4, kDL_ISO8583_BCD_RIGHT  }, // Date, Conversion
    /*  17 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    4, kDL_ISO8583_BCD_RIGHT  }, // Date, Capture
    /*  18 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    4, kDL_ISO8583_BCD_RIGHT  }, // Merchant Type
    /*  19 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    3, kDL_ISO8583_BCD_RIGHT  }, // Country Code, Acquiring Inst
    /*  20 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    3, kDL_ISO8583_BCD_RIGHT  }, // Country Code, Primary Account Number
    /*  21 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    3, kDL_ISO8583_BCD_RIGHT  }, // Country Code, Forwarding Inst
    /*  22 */ {kDL_ISO8583_BCD_RIGHT  ,      0,   12, kDL_ISO8583_ASCII      }, // Point of Service Data Code
    /*  23 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    3, kDL_ISO8583_BCD_RIGHT  }, // Card Sequence Number
    /*  24 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    3, kDL_ISO8583_BCD_RIGHT  }, // Function Code
    /*  25 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    4, kDL_ISO8583_BCD_RIGHT  }, // Message Reason Code
    /*  26 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    4, kDL_ISO8583_BCD_RIGHT  }, // Card Acceptor Business Code
    /*  27 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    1, kDL_ISO8583_BCD_RIGHT  }, // Approval Code Length
    /*  28 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    6, kDL_ISO8583_BCD_RIGHT  }, // Date, Reconciliation
    /*  29 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    3, kDL_ISO8583_BCD_RIGHT  }, // Reconciliation Indicator
    /*  30 */ {kDL_ISO8583_BCD_RIGHT  ,      0,   24, kDL_ISO8583_BCD_RIGHT  }, // Amounts, Original
    /*  31 */ {kDL_ISO8583_BCD_RIGHT  ,      1,   99, kDL_ISO8583_ASCII      }, // Acquirer Reference Data
    /*  32 */ {kDL_ISO8583_BCD_RIGHT  ,      1,   11, kDL_ISO8583_BCD_RIGHT  }, // Acquirer Inst Id Code
    /*  33 */ {kDL_ISO8583_BCD_RIGHT  ,      1,   11, kDL_ISO8583_BCD_RIGHT  }, // Forwarding Inst Id Code
    /*  34 */ {kDL_ISO8583_BCD_RIGHT  ,      1,   28, kDL_ISO8583_ASCII      }, // Primary Account Number, Extended
    /*  35 */ {kDL_ISO8583_BCD_RIGHT  ,      1,   37, kDL_ISO8583_NIBBLE_LEFT  }, // Track 2 Data
    /*  36 */ {kDL_ISO8583_BCD_RIGHT  ,      2,  104, kDL_ISO8583_NIBBLE_LEFT  }, // Track 3 Data
    /*  37 */ {kDL_ISO8583_BCD_RIGHT  ,      0,   12, kDL_ISO8583_ASCII  }, // Retrieval Reference Number
    /*  38 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    6, kDL_ISO8583_ASCII  }, // Approval Code
    /*  39 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    3, kDL_ISO8583_BCD_LEFT  }, // Action Code
    /*  40 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    3, kDL_ISO8583_BCD_LEFT  }, // Service Code
    /*  41 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    8,kDL_ISO8583_ASCII  }, // Card Acceptor Terminal Id
    /*  42 */ {kDL_ISO8583_BCD_RIGHT  ,      0,   15,kDL_ISO8583_ASCII  }, // Card Acceptor Id Code
    /*  43 */ {kDL_ISO8583_BCD_RIGHT  ,      1,   99,kDL_ISO8583_ASCII  }, // Card Acceptor Name/Location
    /*  44 */ {kDL_ISO8583_BCD_RIGHT  ,      1,   99,kDL_ISO8583_ASCII  }, // Additional Response Data
    /*  45 */ {kDL_ISO8583_BCD_RIGHT  ,      1,   76,kDL_ISO8583_ASCII  }, // Track 1 Data
    /*  46 */ {kDL_ISO8583_BCD_RIGHT  ,      2,  204,kDL_ISO8583_ASCII }, // Amounts, Fees
    /*  47 */ {kDL_ISO8583_BCD_RIGHT  ,      2,  999,kDL_ISO8583_ASCII }, // Additional Data - National
    /*  48 */ {kDL_ISO8583_BCD_RIGHT  ,      2,  999,kDL_ISO8583_ASCII }, // Additional Data - Private
    /*  49 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    3,kDL_ISO8583_ASCII  }, // Currency Code, Txn
    /*  50 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    3,kDL_ISO8583_ASCII  }, // Currency Code, Reconciliation
    /*  51 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    3,kDL_ISO8583_ASCII  }, // Currency Code, Cardholder Billing
    /*  52 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    8,kDL_ISO8583_BYTE  }, // Personal Id Number (PIN) Data
    /*  53 */ {kDL_ISO8583_BCD_RIGHT  ,      1,   48,kDL_ISO8583_BYTE  }, // Security Related Control Information
    /*  54 */ {kDL_ISO8583_BCD_RIGHT  ,      2,  120,kDL_ISO8583_ASCII }, // Amounts, Additional
    /*  55 */ {kDL_ISO8583_BCD_RIGHT  ,      2,  255,kDL_ISO8583_BYTE }, // IC Card System Related Data
    /*  56 */ {kDL_ISO8583_BCD_RIGHT  ,      1,   35,kDL_ISO8583_BCD_LEFT  }, // Original Data Elements
    /*  57 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    3,kDL_ISO8583_BCD_LEFT  }, // Authorization Life Cycle Code
    /*  58 */ {kDL_ISO8583_BCD_RIGHT  ,      1,   11,kDL_ISO8583_BCD_LEFT  }, // Authorizing Agent Inst Id Code
    /*  59 */ {kDL_ISO8583_BCD_RIGHT  ,      2,  999,kDL_ISO8583_ASCII }, // Transport Data
    /*  60 */ {kDL_ISO8583_BCD_RIGHT  ,      2,  999,kDL_ISO8583_ASCII }, // Reserved for National use
    /*  61 */ {kDL_ISO8583_BCD_RIGHT  ,      2,  999,kDL_ISO8583_ASCII }, // Reserved for National use
    /*  62 */ {kDL_ISO8583_BCD_RIGHT  ,      2,  999,kDL_ISO8583_ASCII }, // Reserved for Private use
    /*  63 */ {kDL_ISO8583_BCD_RIGHT  ,      2,  999,kDL_ISO8583_ASCII }, // Reserved for Private use
    /*  64 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    8,kDL_ISO8583_BYTE  }, // Message Authentication Code Field
    /*  65 */ {kDL_ISO8583_BCD_RIGHT  ,      0,    8,kDL_ISO8583_BYTE  }, // Reserved for ISO use
    /*  66 */ {kDL_ISO8583_BCD_RIGHT  ,      2,  204,kDL_ISO8583_ASCII }, // Amounts, Original Fees
    /*  67 */ {kDL_ISO8583_BCD_RIGHT,0  ,  2,kDL_ISO8583_BCD_LEFT  }, // Extended Payment Data
    /*  68 */ {kDL_ISO8583_BCD_RIGHT,0  ,  3,kDL_ISO8583_BCD_LEFT  }, // Country Code, Receiving Inst
    /*  69 */ {kDL_ISO8583_BCD_RIGHT,0  ,  3,kDL_ISO8583_BCD_LEFT  }, // Country Code, Settlement Inst
    /*  70 */ {kDL_ISO8583_BCD_RIGHT,0  ,  3,kDL_ISO8583_BCD_LEFT  }, // Country Code, Authorizing Agent Inst
    /*  71 */ {kDL_ISO8583_BCD_RIGHT,0  ,  6,kDL_ISO8583_BCD_LEFT  }, // Message Number
    /*  72 */ {kDL_ISO8583_BCD_RIGHT,2  ,999,kDL_ISO8583_ASCII }, // Data Record
    /*  73 */ {kDL_ISO8583_BCD_RIGHT,0  ,  6,kDL_ISO8583_BCD_LEFT  }, // Date, Action
    /*  74 */ {kDL_ISO8583_BCD_RIGHT,0  , 10,kDL_ISO8583_BCD_LEFT  }, // Credits, Number
    /*  75 */ {kDL_ISO8583_BCD_RIGHT,0  , 10,kDL_ISO8583_BCD_LEFT  }, // Credits, Reversal Number
    /*  76 */ {kDL_ISO8583_BCD_RIGHT,0  , 10,kDL_ISO8583_BCD_LEFT  }, // Debits, Number
    /*  77 */ {kDL_ISO8583_BCD_RIGHT,0  , 10,kDL_ISO8583_BCD_LEFT  }, // Debits, Reversal Number
    /*  78 */ {kDL_ISO8583_BCD_RIGHT,0  , 10,kDL_ISO8583_BCD_LEFT  }, // Transfer, Number
    /*  79 */ {kDL_ISO8583_BCD_RIGHT,0  , 10,kDL_ISO8583_BCD_LEFT  }, // Transfer, Reversal Number
    /*  80 */ {kDL_ISO8583_BCD_RIGHT,0  , 10,kDL_ISO8583_BCD_LEFT  }, // Inquiries, Number
    /*  81 */ {kDL_ISO8583_BCD_RIGHT,0  , 10,kDL_ISO8583_BCD_LEFT  }, // Authorizations, Number
    /*  82 */ {kDL_ISO8583_BCD_RIGHT,0  , 10,kDL_ISO8583_BCD_LEFT  }, // Inquiries, Reversal Number
    /*  83 */ {kDL_ISO8583_BCD_RIGHT,0  , 10,kDL_ISO8583_BCD_LEFT  }, // Payments, Number
    /*  84 */ {kDL_ISO8583_BCD_RIGHT,0  , 10,kDL_ISO8583_BCD_LEFT  }, // Payments, Reversal Number
    /*  85 */ {kDL_ISO8583_BCD_RIGHT,0  , 10,kDL_ISO8583_BCD_LEFT  }, // Fee Collections, Number
    /*  86 */ {kDL_ISO8583_BCD_RIGHT,0  , 16,kDL_ISO8583_BCD_LEFT  }, // Credits, Amount
    /*  87 */ {kDL_ISO8583_BCD_RIGHT,0  , 16,kDL_ISO8583_BCD_LEFT  }, // Credits, Reversal Amount
    /*  88 */ {kDL_ISO8583_BCD_RIGHT,0  , 16,kDL_ISO8583_BCD_LEFT  }, // Debits, Amount
    /*  89 */ {kDL_ISO8583_BCD_RIGHT,0  , 16,kDL_ISO8583_BCD_LEFT  }, // Debits, Reversal Amount
    /*  90 */ {kDL_ISO8583_BCD_RIGHT,0  , 10,kDL_ISO8583_BCD_LEFT  }, // Authorizations, Reversal Number
    /*  91 */ {kDL_ISO8583_BCD_RIGHT,0  ,  3,kDL_ISO8583_BCD_LEFT  }, // Country Code, Txn Destination Inst
    /*  92 */ {kDL_ISO8583_BCD_RIGHT,0  ,  3,kDL_ISO8583_BCD_LEFT  }, // Country Code, Txn Originator Inst
    /*  93 */ {kDL_ISO8583_BCD_RIGHT  , 1,11,kDL_ISO8583_BCD_LEFT  }, // Txn Destination Inst Id Code
    /*  94 */ {kDL_ISO8583_BCD_RIGHT  , 1,11,kDL_ISO8583_BCD_LEFT  }, // Txn Originator Inst Id Code
    /*  95 */ {kDL_ISO8583_BCD_RIGHT, 1,99,kDL_ISO8583_ASCII  }, // Card Issuer Reference Data
    /*  96 */ {kDL_ISO8583_BCD_RIGHT  , 1,999,kDL_ISO8583_BYTE }, // Key Management Data
    /*  97 */ {kDL_ISO8583_BCD_RIGHT , 0,17,kDL_ISO8583_NIBBLE_RIGHT  }, // Amount, Net Reconciliation
    /*  98 */ {kDL_ISO8583_BCD_RIGHT, 0,25,kDL_ISO8583_ASCII  }, // Payee
    /*  99 */ {kDL_ISO8583_BCD_RIGHT , 1,11,kDL_ISO8583_ASCII  }, // Settlement Inst Id Code
    /* 100 */ {kDL_ISO8583_BCD_RIGHT , 1,11,kDL_ISO8583_BCD_LEFT  }, // Receiving Inst Id Code
    /* 101 */ {kDL_ISO8583_BCD_RIGHT, 1,17,kDL_ISO8583_ASCII  }, // File Name
    /* 102 */ {kDL_ISO8583_BCD_RIGHT, 1,28,kDL_ISO8583_ASCII  }, // Account Id 1
    /* 103 */ {kDL_ISO8583_BCD_RIGHT, 1,28,kDL_ISO8583_ASCII  }, // Account Id 2
    /* 104 */ {kDL_ISO8583_BCD_RIGHT, 2,100,kDL_ISO8583_ASCII }, // Txn Description
    /* 105 */ {kDL_ISO8583_BCD_RIGHT , 0,16,kDL_ISO8583_BCD_RIGHT }, // Credits, Chargeback Amount
    /* 106 */ {kDL_ISO8583_BCD_RIGHT , 0,16,kDL_ISO8583_BCD_RIGHT  }, // Debits, Chargeback Amount
    /* 107 */ {kDL_ISO8583_BCD_RIGHT , 0,10,kDL_ISO8583_BCD_RIGHT  }, // Credits, Chargeback Number
    /* 108 */ {kDL_ISO8583_BCD_RIGHT , 0,10,kDL_ISO8583_BCD_RIGHT  }, // Debits, Chargeback Number
    /* 109 */ {kDL_ISO8583_BCD_RIGHT, 1,84,kDL_ISO8583_ASCII  }, // Credits, Fee Amounts
    /* 110 */ {kDL_ISO8583_BCD_RIGHT, 1,84,kDL_ISO8583_ASCII  }, // Debits, Fee Amounts
    /* 111 */ {kDL_ISO8583_BCD_RIGHT,2,999,kDL_ISO8583_ASCII }, // Reserved for ISO use
    /* 112 */ {kDL_ISO8583_BCD_RIGHT,2,999,kDL_ISO8583_ASCII }, // Reserved for ISO use
    /* 113 */ {kDL_ISO8583_BCD_RIGHT,2,999,kDL_ISO8583_ASCII }, // Reserved for ISO use
    /* 114 */ {kDL_ISO8583_BCD_RIGHT,2,999,kDL_ISO8583_ASCII }, // Reserved for ISO use
    /* 115 */ {kDL_ISO8583_BCD_RIGHT,2,999,kDL_ISO8583_ASCII }, // Reserved for ISO use
    /* 116 */ {kDL_ISO8583_BCD_RIGHT,2,999,kDL_ISO8583_ASCII }, // Reserved for National use
    /* 117 */ {kDL_ISO8583_BCD_RIGHT,2,999,kDL_ISO8583_ASCII }, // Reserved for National use
    /* 118 */ {kDL_ISO8583_BCD_RIGHT,2,999,kDL_ISO8583_ASCII }, // Reserved for National use
    /* 119 */ {kDL_ISO8583_BCD_RIGHT,2,999,kDL_ISO8583_ASCII }, // Reserved for National use
    /* 120 */ {kDL_ISO8583_BCD_RIGHT,2,999,kDL_ISO8583_ASCII }, // Reserved for National use
    /* 121 */ {kDL_ISO8583_BCD_RIGHT,2,999,kDL_ISO8583_ASCII }, // Reserved for National use
    /* 122 */ {kDL_ISO8583_BCD_RIGHT,2,999,kDL_ISO8583_ASCII }, // Reserved for National use
    /* 123 */ {kDL_ISO8583_BCD_RIGHT,2,999,kDL_ISO8583_ASCII }, // Reserved for Private use
    /* 124 */ {kDL_ISO8583_BCD_RIGHT,2,999,kDL_ISO8583_ASCII }, // Reserved for Private use
    /* 125 */ {kDL_ISO8583_BCD_RIGHT,2,999,kDL_ISO8583_ASCII }, // Reserved for Private use
    /* 126 */ {kDL_ISO8583_BCD_RIGHT,2,999,kDL_ISO8583_ASCII }, // Reserved for Private use
    /* 127 */ {kDL_ISO8583_BCD_RIGHT,2,999,kDL_ISO8583_ASCII }, // Reserved for Private use
    /* 128 */ {kDL_ISO8583_BCD_RIGHT,0,  8,kDL_ISO8583_BYTE  }  // Message Authentication Code Field
};

/******************************************************************************/

void DL_ISO8583_DEFS_1993_GetHandler ( DL_ISO8583_HANDLER *oHandler )
{
        DL_ISO8583_COMMON_SetHandler(
            _iso8583_1993_fields,
            (DL_UINT8)(sizeof(_iso8583_1993_fields)/sizeof(DL_ISO8583_FIELD_DEF)),
            oHandler);
    return;
}

/******************************************************************************/
