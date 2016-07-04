# Copyright (C) 2008-2016, Marvell International Ltd.
# All Rights Reserved.

exec-y += modbus_master
modbus_master-objs-y := src/main.c src/modbus.c src/modbus-data.c src/modbus-rtu.c

exec-y += bandwidth-client
bandwidth-client-objs-y := src/bandwidth-client.c src/modbus.c src/modbus-data.c src/modbus-rtu.c

exec-y += bandwidth-server-one
bandwidth-server-one-objs-y := src/bandwidth-server-one.c src/modbus.c src/modbus-data.c src/modbus-rtu.c


#Application specific entities can be specified as follows
#<app-name>-board-y := /path/to/boardfile
#<app-name>-linkerscript-y := /path/to/linkerscript
