/*
 * Copyright © 2001-2011 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef _MODBUS_RTU_PRIVATE_H_
#define _MODBUS_RTU_PRIVATE_H_

#include <stdint.h>
#include <mdev_uart.h>

#define _MODBUS_RTU_HEADER_LENGTH      1
#define _MODBUS_RTU_PRESET_REQ_LENGTH  6
#define _MODBUS_RTU_PRESET_RSP_LENGTH  2

#define _MODBUS_RTU_CHECKSUM_LENGTH    2

typedef struct _modbus_rtu {
    /* Device: "/dev/ttyS0", "/dev/ttyUSB0" or "/dev/tty.USA19*" on Mac OS X for
       KeySpan USB<->Serial adapters this string had to be made bigger on OS X
       as the directory+file name was bigger than 19 bytes. Making it 67 bytes
       for now, but OS X does support 256 byte file names. May become a problem
       in the future. */
    int port;
    /* Bauds: 9600, 19200, 57600, 115200, etc */
    int baud;
    /* Data bit */
    int data_bit;
    /* Stop bit */
    int stop_bit;
    /* Parity: 'N', 'O', 'E' */
    char parity;

    mdev_t *dev;
} modbus_rtu_t;

#endif /* _MODBUS_RTU_PRIVATE_H_ */
