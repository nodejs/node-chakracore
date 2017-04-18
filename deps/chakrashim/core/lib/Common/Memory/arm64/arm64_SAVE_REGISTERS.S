// -------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
// -------------------------------------------------------------------------------------------------------

#include "unixasmmacros.inc"


    LEAF_ENTRY arm64_SAVE_REGISTERS

    ; Can't use sp with stp so mov to a volatile register
    ; and then store onto passed in array
    mov   x16, sp
    str   x16,      [x0, #0x00]
    str   x1,       [x0, #0x08]
    stp   x2,  x3,  [x0, #0x10]
    stp   x4,  x5,  [x0, #0x20]
    stp   x6,  x7,  [x0, #0x30]
    stp   x8,  x9,  [x0, #0x40]
    stp   x10, x11, [x0, #0x50]
    str   x12,      [x0, #0x60]
    ;stp   x13, x14, [x0, #0x70]
    ;stp   x15, x17, [x0, #0x80]
    ;stp   x18, x19, [x0, #0x90]
    ;stp   x20, x21, [x0, #0xA0]
    ;stp   x22, x23, [x0, #0xB0]
    ;stp   x24, x25, [x0, #0xC0]
    ;stp   x26, x27, [x0, #0xD0]
    ;str   x28,      [x0, #0xE0]

    br      lr

    LEAF_END arm64_SAVE_REGISTERS
