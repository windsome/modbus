/*
 * Copyright © 2008-2010 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <wmstdio.h>
#include <string.h>
#include <stdlib.h>

#include "modbus.h"
#include "modbus-rtu.h"

int main(int argc, char *argv[])
{
    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;
    int rc;

    /* Initialize wmstdio console on UART0 */
    wmstdio_init(UART0_ID, 0);

    wmprintf("begin bandwidth-server-one\r\n");

    ctx = modbus_new_rtu(UART1_ID, 115200, UART_8BIT);
    modbus_set_slave(ctx, 1);
    modbus_connect(ctx);

    mb_mapping = modbus_mapping_new(MODBUS_MAX_READ_BITS, 0,
                                    MODBUS_MAX_READ_REGISTERS, 0);
    if (mb_mapping == NULL) {
        wmprintf("Failed to allocate the mapping\r\n");
        modbus_free(ctx);
        return -1;
    }

    for(;;) {
        uint8_t query[MODBUS_RTU_MAX_ADU_LENGTH];

        rc = modbus_receive(ctx, query);
        if (rc >= 0) {
            modbus_reply(ctx, query, rc, mb_mapping);
        } else {
            wmprintf("Something wrong!\r\n");
            //wmprintf("Connection closed by the client or server\r\n");
            
            //break;
        }
    }

    wmprintf("Quit the loop\r\n");

    modbus_mapping_free(mb_mapping);
    modbus_free(ctx);

    return 0;
}
