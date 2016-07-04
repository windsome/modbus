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
#include <mdev_uart.h>
#include <wm_os.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "modbus.h"
#include "modbus-rtu.h"
//#include "../src/modbus.h"
//#include "../src/modbus-rtu.h"

#define G_MSEC_PER_SEC 1000

uint32_t gettime_ms(void)
{
    return 0;
    //return xTaskGetTickCount ();
#if 0
    struct timeval tv;
    gettimeofday (&tv, NULL);

    return (uint32_t) tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
}

/* Tests based on PI-MBUS-300 documentation */
int main(int argc, char *argv[])
{
    uint8_t *tab_bit;
    uint16_t *tab_reg;
    modbus_t *ctx;
    int i;
    int nb_points;
    double elapsed;
    uint32_t start;
    uint32_t end;
    uint32_t bytes;
    uint32_t rate;
    int rc;

    int n_loop;
    n_loop = 100;

    /* Initialize wmstdio console on UART0 */
    wmstdio_init(UART0_ID, 0);

    wmprintf("begin bandwidth-client\r\n");

    ctx = modbus_new_rtu(UART1_ID, 115200, UART_8BIT);
    modbus_set_slave(ctx, 1);

    if (modbus_connect(ctx) == -1) {
        wmprintf("Connexion failed\r\n");
        modbus_free(ctx);
        return -1;
    }

    /* Allocate and initialize the memory to store the status */
    tab_bit = (uint8_t *) os_mem_alloc(MODBUS_MAX_READ_BITS * sizeof(uint8_t));
    memset(tab_bit, 0, MODBUS_MAX_READ_BITS * sizeof(uint8_t));

    /* Allocate and initialize the memory to store the registers */
    tab_reg = (uint16_t *) os_mem_alloc(MODBUS_MAX_READ_REGISTERS * sizeof(uint16_t));
    memset(tab_reg, 0, MODBUS_MAX_READ_REGISTERS * sizeof(uint16_t));

    wmprintf("READ BITS\r\n\r\n");

    nb_points = MODBUS_MAX_READ_BITS;
    start = gettime_ms();
    for (i=0; i<n_loop; i++) {
        rc = modbus_read_bits(ctx, 0, nb_points, tab_bit);
        if (rc == -1) {
            wmprintf("error modbus_read_bits\r\n");
            //return -1;
        }
    }
    end = gettime_ms();
    elapsed = end - start;

    rate = (n_loop * nb_points) * G_MSEC_PER_SEC / (end - start);
    wmprintf("Transfert rate in points/seconds:\r\n");
    wmprintf("* %d points/s\r\n", rate);
    wmprintf("\r\n");

    bytes = n_loop * (nb_points / 8) + ((nb_points % 8) ? 1 : 0);
    rate = bytes / 1024 * G_MSEC_PER_SEC / (end - start);
    wmprintf("Values:\r\n");
    wmprintf("* %d x %d values\r\n", n_loop, nb_points);
    wmprintf("* %.3f ms for %d bytes\r\n", elapsed, bytes);
    wmprintf("* %d KiB/s\r\n", rate);
    wmprintf("\r\n");

    /* TCP: Query and reponse header and values */
    bytes = 12 + 9 + (nb_points / 8) + ((nb_points % 8) ? 1 : 0);
    wmprintf("Values and TCP Modbus overhead:\r\n");
    wmprintf("* %d x %d bytes\r\n", n_loop, bytes);
    bytes = n_loop * bytes;
    rate = bytes / 1024 * G_MSEC_PER_SEC / (end - start);
    wmprintf("* %.3f ms for %d bytes\r\n", elapsed, bytes);
    wmprintf("* %d KiB/s\r\n", rate);
    wmprintf("\r\n\r\n");

    wmprintf("READ REGISTERS\r\n\r\n");

    nb_points = MODBUS_MAX_READ_REGISTERS;
    start = gettime_ms();
    for (i=0; i<n_loop; i++) {
        rc = modbus_read_registers(ctx, 0, nb_points, tab_reg);
        if (rc == -1) {
            wmprintf("error modbus_read_registers\r\n");
            //return -1;
        }
    }
    end = gettime_ms();
    elapsed = end - start;

    rate = (n_loop * nb_points) * G_MSEC_PER_SEC / (end - start);
    wmprintf("Transfert rate in points/seconds:\r\n");
    wmprintf("* %d registers/s\r\n", rate);
    wmprintf("\r\n");

    bytes = n_loop * nb_points * sizeof(uint16_t);
    rate = bytes / 1024 * G_MSEC_PER_SEC / (end - start);
    wmprintf("Values:\r\n");
    wmprintf("* %d x %d values\r\n", n_loop, nb_points);
    wmprintf("* %.3f ms for %d bytes\r\n", elapsed, bytes);
    wmprintf("* %d KiB/s\r\n", rate);
    wmprintf("\r\n");

    /* TCP:Query and reponse header and values */
    bytes = 12 + 9 + (nb_points * sizeof(uint16_t));
    wmprintf("Values and TCP Modbus overhead:\r\n");
    wmprintf("* %d x %d bytes\r\n", n_loop, bytes);
    bytes = n_loop * bytes;
    rate = bytes / 1024 * G_MSEC_PER_SEC / (end - start);
    wmprintf("* %.3f ms for %d bytes\r\n", elapsed, bytes);
    wmprintf("* %d KiB/s\r\n", rate);
    wmprintf("\r\n\r\n");

    wmprintf("READ AND WRITE REGISTERS\r\n\r\n");

    nb_points = MODBUS_MAX_RW_WRITE_REGISTERS;
    start = gettime_ms();
    for (i=0; i<n_loop; i++) {
        rc = modbus_write_and_read_registers(ctx,
                                             0, nb_points, tab_reg,
                                             0, nb_points, tab_reg);
        if (rc == -1) {
            wmprintf("error modbus_write_and_read_registers\r\n");
            //return -1;
        }
    }
    end = gettime_ms();
    elapsed = end - start;

    rate = (n_loop * nb_points) * G_MSEC_PER_SEC / (end - start);
    wmprintf("Transfert rate in points/seconds:\r\n");
    wmprintf("* %d registers/s\r\n", rate);
    wmprintf("\r\n");

    bytes = n_loop * nb_points * sizeof(uint16_t);
    rate = bytes / 1024 * G_MSEC_PER_SEC / (end - start);
    wmprintf("Values:\r\n");
    wmprintf("* %d x %d values\r\n", n_loop, nb_points);
    wmprintf("* %.3f ms for %d bytes\r\n", elapsed, bytes);
    wmprintf("* %d KiB/s\r\n", rate);
    wmprintf("\r\n");

    /* TCP:Query and reponse header and values */
    bytes = 12 + 9 + (nb_points * sizeof(uint16_t));
    wmprintf("Values and TCP Modbus overhead:\r\n");
    wmprintf("* %d x %d bytes\r\n", n_loop, bytes);
    bytes = n_loop * bytes;
    rate = bytes / 1024 * G_MSEC_PER_SEC / (end - start);
    wmprintf("* %.3f ms for %d bytes\r\n", elapsed, bytes);
    wmprintf("* %d KiB/s\r\n", rate);
    wmprintf("\r\n");

    /* Free the memory */
    os_mem_free(tab_bit);
    os_mem_free(tab_reg);

    /* Close the connection */
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}
