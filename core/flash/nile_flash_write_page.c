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
#include "nile/flash.h"
#include "nile/hardware.h"
#include "nile/spi.h"
#include "utils.h"

bool nile_flash_write_page(const void __far* buffer, uint32_t address, uint16_t size) {
    bool result = false;
    uint8_t cmd[4];
    cmd[0] = NILE_FLASH_CMD_WRITE;
    cmd[1] = address >> 16;
    cmd[2] = address >> 8;
    cmd[3] = address;

    if (!nile_flash_write_enable())
        goto error;

    nile_spi_init_flash_cs_low();

    if (!nile_spi_tx_async_block(cmd, 4))
        goto error;

    if (!nile_spi_tx_sync_block(buffer, size))
        goto error;

    if (!nile_spi_wait_ready())
        goto error;

    nile_spi_init_flash_cs_high();

    if (!nile_flash_wait_ready())
        goto error;

    result = true;

error:
    nile_spi_init_flash_cs_high();

    return result;
}
