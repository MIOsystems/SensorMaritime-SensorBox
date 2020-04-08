################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dave/Generated/ETH_LWIP/lwip/core/altcp.c \
../Dave/Generated/ETH_LWIP/lwip/core/altcp_alloc.c \
../Dave/Generated/ETH_LWIP/lwip/core/altcp_tcp.c \
../Dave/Generated/ETH_LWIP/lwip/core/def.c \
../Dave/Generated/ETH_LWIP/lwip/core/dns.c \
../Dave/Generated/ETH_LWIP/lwip/core/inet_chksum.c \
../Dave/Generated/ETH_LWIP/lwip/core/init.c \
../Dave/Generated/ETH_LWIP/lwip/core/ip.c \
../Dave/Generated/ETH_LWIP/lwip/core/mem.c \
../Dave/Generated/ETH_LWIP/lwip/core/memp.c \
../Dave/Generated/ETH_LWIP/lwip/core/netif.c \
../Dave/Generated/ETH_LWIP/lwip/core/pbuf.c \
../Dave/Generated/ETH_LWIP/lwip/core/raw.c \
../Dave/Generated/ETH_LWIP/lwip/core/stats.c \
../Dave/Generated/ETH_LWIP/lwip/core/sys.c \
../Dave/Generated/ETH_LWIP/lwip/core/tcp.c \
../Dave/Generated/ETH_LWIP/lwip/core/tcp_in.c \
../Dave/Generated/ETH_LWIP/lwip/core/tcp_out.c \
../Dave/Generated/ETH_LWIP/lwip/core/timeouts.c \
../Dave/Generated/ETH_LWIP/lwip/core/udp.c 

OBJS += \
./Dave/Generated/ETH_LWIP/lwip/core/altcp.o \
./Dave/Generated/ETH_LWIP/lwip/core/altcp_alloc.o \
./Dave/Generated/ETH_LWIP/lwip/core/altcp_tcp.o \
./Dave/Generated/ETH_LWIP/lwip/core/def.o \
./Dave/Generated/ETH_LWIP/lwip/core/dns.o \
./Dave/Generated/ETH_LWIP/lwip/core/inet_chksum.o \
./Dave/Generated/ETH_LWIP/lwip/core/init.o \
./Dave/Generated/ETH_LWIP/lwip/core/ip.o \
./Dave/Generated/ETH_LWIP/lwip/core/mem.o \
./Dave/Generated/ETH_LWIP/lwip/core/memp.o \
./Dave/Generated/ETH_LWIP/lwip/core/netif.o \
./Dave/Generated/ETH_LWIP/lwip/core/pbuf.o \
./Dave/Generated/ETH_LWIP/lwip/core/raw.o \
./Dave/Generated/ETH_LWIP/lwip/core/stats.o \
./Dave/Generated/ETH_LWIP/lwip/core/sys.o \
./Dave/Generated/ETH_LWIP/lwip/core/tcp.o \
./Dave/Generated/ETH_LWIP/lwip/core/tcp_in.o \
./Dave/Generated/ETH_LWIP/lwip/core/tcp_out.o \
./Dave/Generated/ETH_LWIP/lwip/core/timeouts.o \
./Dave/Generated/ETH_LWIP/lwip/core/udp.o 

C_DEPS += \
./Dave/Generated/ETH_LWIP/lwip/core/altcp.d \
./Dave/Generated/ETH_LWIP/lwip/core/altcp_alloc.d \
./Dave/Generated/ETH_LWIP/lwip/core/altcp_tcp.d \
./Dave/Generated/ETH_LWIP/lwip/core/def.d \
./Dave/Generated/ETH_LWIP/lwip/core/dns.d \
./Dave/Generated/ETH_LWIP/lwip/core/inet_chksum.d \
./Dave/Generated/ETH_LWIP/lwip/core/init.d \
./Dave/Generated/ETH_LWIP/lwip/core/ip.d \
./Dave/Generated/ETH_LWIP/lwip/core/mem.d \
./Dave/Generated/ETH_LWIP/lwip/core/memp.d \
./Dave/Generated/ETH_LWIP/lwip/core/netif.d \
./Dave/Generated/ETH_LWIP/lwip/core/pbuf.d \
./Dave/Generated/ETH_LWIP/lwip/core/raw.d \
./Dave/Generated/ETH_LWIP/lwip/core/stats.d \
./Dave/Generated/ETH_LWIP/lwip/core/sys.d \
./Dave/Generated/ETH_LWIP/lwip/core/tcp.d \
./Dave/Generated/ETH_LWIP/lwip/core/tcp_in.d \
./Dave/Generated/ETH_LWIP/lwip/core/tcp_out.d \
./Dave/Generated/ETH_LWIP/lwip/core/timeouts.d \
./Dave/Generated/ETH_LWIP/lwip/core/udp.d 


# Each subdirectory must supply rules for building sources it contributes
Dave/Generated/ETH_LWIP/lwip/core/%.o: ../Dave/Generated/ETH_LWIP/lwip/core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4800_F144x2048 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)/Libraries/CMSIS/Include" -I"$(PROJECT_LOC)/Libraries/CMSIS/Infineon/XMC4800_series/Include" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Dave/Generated" -I"$(PROJECT_LOC)/Libraries" -O0 -ffunction-sections -fdata-sections -Wall -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

