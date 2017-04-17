// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "asmconstants.h"
#include "unixasmmacros.inc"

// LPVOID __stdcall GetCurrentIP(void)//
LEAF_ENTRY GetCurrentIP, _TEXT
    mov x0, lr
    ret lr
LEAF_END GetCurrentIP, _TEXT

// LPVOID __stdcall GetCurrentSP(void)//
LEAF_ENTRY GetCurrentSP, _TEXT
    mov x0, sp
    ret lr
LEAF_END GetCurrentSP, _TEXT
