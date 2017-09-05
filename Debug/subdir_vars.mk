################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../subscribe_publish_sample.cfg 

CMD_SRCS += \
../CC3200_LAUNCHXL.cmd 

C_SRCS += \
../CC3200_LAUNCHXL.c \
../UARTUtils.c \
../main.c \
../netwifi.c \
../rom_pin_mux_config.c \
../startsntp.c \
../startupccs.c \
../subscribe_publish_sample.c 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_MISC_DIRS += \
./configPkg/ 

C_DEPS += \
./CC3200_LAUNCHXL.d \
./UARTUtils.d \
./main.d \
./netwifi.d \
./rom_pin_mux_config.d \
./startsntp.d \
./startupccs.d \
./subscribe_publish_sample.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./CC3200_LAUNCHXL.obj \
./UARTUtils.obj \
./main.obj \
./netwifi.obj \
./rom_pin_mux_config.obj \
./startsntp.obj \
./startupccs.obj \
./subscribe_publish_sample.obj 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

OBJS__QUOTED += \
"CC3200_LAUNCHXL.obj" \
"UARTUtils.obj" \
"main.obj" \
"netwifi.obj" \
"rom_pin_mux_config.obj" \
"startsntp.obj" \
"startupccs.obj" \
"subscribe_publish_sample.obj" 

C_DEPS__QUOTED += \
"CC3200_LAUNCHXL.d" \
"UARTUtils.d" \
"main.d" \
"netwifi.d" \
"rom_pin_mux_config.d" \
"startsntp.d" \
"startupccs.d" \
"subscribe_publish_sample.d" 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

C_SRCS__QUOTED += \
"../CC3200_LAUNCHXL.c" \
"../UARTUtils.c" \
"../main.c" \
"../netwifi.c" \
"../rom_pin_mux_config.c" \
"../startsntp.c" \
"../startupccs.c" \
"../subscribe_publish_sample.c" 


