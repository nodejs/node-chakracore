// -------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
// -------------------------------------------------------------------------------------------------------

#include "unixasmmacros.inc"

.global C_FUNC(_ZN2Js13ScriptContext31ProfileModeDeferredParsingThunkEPNS_16RecyclableObjectENS_8CallInfoEz)
.global C_FUNC(_ZN2Js13ScriptContext35ProfileModeDeferredDeserializeThunkEPNS_16RecyclableObjectENS_8CallInfoEz)

NESTED_ENTRY _ZN2Js13ScriptContext31ProfileModeDeferredParsingThunkEPNS_16RecyclableObjectENS_8CallInfoEz, _TEXT, NoHandler

    PROLOG_SAVE_REG_PAIR fp, lr, -80 ; save volatile registers

    stp   x0, x1, [sp, 16]
    stp   x2, x3, [sp, 32]
    stp   x4, x5, [sp, 48]
    stp   x6, x7, [sp, 64]

    mov  x0, sp                   ; Pass the address of the function at the saved x0 in case it need to be boxed
    bl   C_FUNC(_ZN2Js13ScriptContext24ProfileModeDeferredParseEPPNS_14ScriptFunctionE) ; call ScriptContext::ProfileModeDeferredParse
    mov  x16, x0                  ; back up entryPoint in x16

    ldp   x6, x7, [sp, 64] ; restore arguments and return address
    ldp   x4, x5, [sp, 48]
    ldp   x2, x3, [sp, 32]
    ldp   x0, x1, [sp, 16]

    EPILOG_RESTORE_REG_PAIR fp, lr, 80

    br x16             ; jump (tail call) to new entryPoint

NESTED_END _ZN2Js13ScriptContext31ProfileModeDeferredParsingThunkEPNS_16RecyclableObjectENS_8CallInfoEz, _TEXT

NESTED_ENTRY _ZN2Js13ScriptContext35ProfileModeDeferredDeserializeThunkEPNS_16RecyclableObjectENS_8CallInfoEz, _TEXT, NoHandler

    PROLOG_SAVE_REG_PAIR fp, lr, -80 ; save volatile registers

    stp   x0, x1, [sp, 16]
    stp   x2, x3, [sp, 32]
    stp   x4, x5, [sp, 48]
    stp   x6, x7, [sp, 64]

    bl C_FUNC(_ZN2Js13ScriptContext30ProfileModeDeferredDeserializeEPNS_14ScriptFunctionE)
    mov  x16, x0                  ; back up entryPoint in x16

    ldp   x6, x7, [sp, 64] ; restore arguments and return address
    ldp   x4, x5, [sp, 48]
    ldp   x2, x3, [sp, 32]
    ldp   x0, x1, [sp, 16]
    EPILOG_RESTORE_REG_PAIR fp, lr, 80
    br x16             ; jump (tail call) to new entryPoint

NESTED_END _ZN2Js13ScriptContext35ProfileModeDeferredDeserializeThunkEPNS_16RecyclableObjectENS_8CallInfoEz, _TEXT
