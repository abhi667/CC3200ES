################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
tirtos/network_sl.obj: C:/Users/Abhijit/Desktop/AWS/J_CC3200ES3_Virginia_1212df3e4c/platform/tirtos/network_sl.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/Abhijit/Desktop/AWS/J_CC3200ES3_Virginia_1212df3e4c/platform/tirtos" --include_path="C:/ti/tirtos_cc32xx_2_16_01_14/products/tidrivers_cc32xx_2_16_01_13/packages/ti/mw/wifi/cc3x00/oslib" --include_path="C:/Users/Abhijit/Desktop/AWS/J_CC3200ES3_Virginia_1212df3e4c/external_libs/jsmn" --include_path="C:/ti/tirtos_cc32xx_2_16_01_14/products/tidrivers_cc32xx_2_16_01_13/packages/ti/mw/wifi/cc3x00/simplelink/include" --include_path="C:/ti/tirtos_cc32xx_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix" --include_path="C:/AWS_CC3200ES3_Publish_Subscribe" --include_path="C:/AWS_CC3200ES3_Publish_Subscribe" --include_path="C:/Users/Abhijit/Desktop/AWS/J_CC3200ES3_Virginia_1212df3e4c/include" --include_path="C:/ti/tirtos_cc32xx_2_16_01_14/products/CC3200_driverlib_1.1.0" --include_path="C:/ti/tirtos_cc32xx_2_16_01_14/products/CC3200_driverlib_1.1.0/inc" --include_path="C:/ti/tirtos_cc32xx_2_16_01_14/products/CC3200_driverlib_1.1.0/driverlib" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --define=ENABLE_IOT_ERROR --define=ENABLE_IOT_DEBUG --define=ENABLE_IOT_WARN --define=ENABLE_IOT_INFO --define=NET_SL --define=cc3200 --define=PART_CC3200 --define=ccs --define=CCWARE -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="tirtos/network_sl.d" --obj_directory="tirtos" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

tirtos/timer.obj: C:/Users/Abhijit/Desktop/AWS/J_CC3200ES3_Virginia_1212df3e4c/platform/tirtos/timer.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/Abhijit/Desktop/AWS/J_CC3200ES3_Virginia_1212df3e4c/platform/tirtos" --include_path="C:/ti/tirtos_cc32xx_2_16_01_14/products/tidrivers_cc32xx_2_16_01_13/packages/ti/mw/wifi/cc3x00/oslib" --include_path="C:/Users/Abhijit/Desktop/AWS/J_CC3200ES3_Virginia_1212df3e4c/external_libs/jsmn" --include_path="C:/ti/tirtos_cc32xx_2_16_01_14/products/tidrivers_cc32xx_2_16_01_13/packages/ti/mw/wifi/cc3x00/simplelink/include" --include_path="C:/ti/tirtos_cc32xx_2_16_01_14/products/bios_6_45_02_31/packages/ti/sysbios/posix" --include_path="C:/AWS_CC3200ES3_Publish_Subscribe" --include_path="C:/AWS_CC3200ES3_Publish_Subscribe" --include_path="C:/Users/Abhijit/Desktop/AWS/J_CC3200ES3_Virginia_1212df3e4c/include" --include_path="C:/ti/tirtos_cc32xx_2_16_01_14/products/CC3200_driverlib_1.1.0" --include_path="C:/ti/tirtos_cc32xx_2_16_01_14/products/CC3200_driverlib_1.1.0/inc" --include_path="C:/ti/tirtos_cc32xx_2_16_01_14/products/CC3200_driverlib_1.1.0/driverlib" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --define=ENABLE_IOT_ERROR --define=ENABLE_IOT_DEBUG --define=ENABLE_IOT_WARN --define=ENABLE_IOT_INFO --define=NET_SL --define=cc3200 --define=PART_CC3200 --define=ccs --define=CCWARE -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="tirtos/timer.d" --obj_directory="tirtos" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


