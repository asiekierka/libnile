/**
 * Copyright (c) 2023, 2024 Adrian "asie" Siekierka
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <wonderful.h>
#include <ws.h>
#include "nile.h"
#include "macros.inc"

    .arch i186
    .code16
    .intel_syntax noprefix

    .section .fartext.s.libnile, "ax"
    .align 2
    .global nile_spi_rx_sync_flip
    // Input:
    // - AX = length
    // - DX = mode
nile_spi_rx_sync_flip:
    IA16_CALL nile_spi_rx_async
    test al, al
    jz 9f
    
    call __nile_spi_wait_ready_near
    test al, al
    jz 9f

    in al, (IO_NILE_SPI_CNT+1)
    xor al, (NILE_SPI_BUFFER_IDX >> 8)
    out (IO_NILE_SPI_CNT+1), al

9:
    jmp nile_spi_abort
