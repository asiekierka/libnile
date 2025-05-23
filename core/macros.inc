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

.macro m_push_sram_bank_state
    // uint8_t prev_flash = inportb(WS_CART_BANK_FLASH_PORT);
    in al, WS_CART_BANK_FLASH_PORT
    push ax
    // uint8_t prev_bank = inportw(WS_CART_EXTBANK_RAM_PORT);
    in ax, WS_CART_EXTBANK_RAM_PORT
    push ax
.endm
.macro m_pop_sram_bank_state
    // outportw(WS_CART_EXTBANK_RAM_PORT, prev_bank);
    pop ax
    out WS_CART_EXTBANK_RAM_PORT, ax
    // outportb(WS_CART_BANK_FLASH_PORT, prev_flash);
    pop ax
    out WS_CART_BANK_FLASH_PORT, al
.endm

.macro m_open_sram_ram_bank bank
    // outportb(WS_CART_BANK_FLASH_PORT, 0);
    xor ax, ax
    out WS_CART_BANK_FLASH_PORT, al
    // outportw(WS_CART_EXTBANK_RAM_PORT, NILE_SEG_RAM_TX);
    mov al, \bank
    out WS_CART_EXTBANK_RAM_PORT, ax
.endm

.macro m_open_sram_rom_bank bank
    // outportb(WS_CART_BANK_FLASH_PORT, 1);
    mov al, 1
    out WS_CART_BANK_FLASH_PORT, al
    // outportw(WS_CART_EXTBANK_RAM_PORT, NILE_SEG_RAM_TX);
    mov ax, \bank
    out WS_CART_EXTBANK_RAM_PORT, ax
.endm

.macro m_nile_spi_wait_ready_al_no_timeout
1:
    in al, (IO_NILE_SPI_CNT+1)
    test al, al
    js 1b
.endm

.macro m_nile_spi_wait_ready_ax_no_timeout
1:
    in ax, IO_NILE_SPI_CNT
    test ah, ah
    js 1b
.endm
