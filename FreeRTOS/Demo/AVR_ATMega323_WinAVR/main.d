main.o main.d : main.c ../../Source/include/FreeRTOS.h FreeRTOSConfig.h \
 ../../Source/include/projdefs.h ../../Source/include/portable.h \
 ../../Source/include/deprecated_definitions.h \
 ../../Source/include/../portable/GCC/ATMega323/portmacro.h \
 ../../Source/include/mpu_wrappers.h ../../Source/include/task.h \
 ../../Source/include/list.h tcp/tcp_server.h \
 ../../../lib/ioLibrary_Driver/Ethernet/socket.h \
 ../../../lib/ioLibrary_Driver/Ethernet/wizchip_select.h \
 ../../../lib/ioLibrary_Driver/Ethernet/wizchip_conf.h \
 ../../../lib/ioLibrary_Driver/Ethernet/W5500/w5500.h \
 ../../../lib/ioLibrary_Driver/Ethernet/W5500/../wizchip_select.h \
 ../../../lib/ioLibrary_Driver/Ethernet/wizchip_conf.h uart.h
