#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/FW-2643-80-04_00.0A.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/FW-2643-80-04_00.0A.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=Global/SoftwareCrc.c Global/global.c Global/provision.c Lib/WirelessHART_Lib.c MS/MS_Data.c MS/MS_Process.c Platform/GlobalVariables.c Platform/gpio.c Platform/interrupt_callbacks.c Platform/measurement.c Platform/nvm.c Platform/timers.c Platform/uart.c Platform/watchdog.c main.c WiredHart/wired_hart_app_layer.c WiredHart/wired_hart_link_layer.c WirelessHART_AppLayer/App_WH_CmdCompose.c WirelessHART_AppLayer/App_WH_CmdExecute.c WirelessHART_AppLayer/App_WH_CmdParse.c WirelessHART_AppLayer/App_WH_User.c WirelessHART_Full_API/wh_full_api.c WirelessHART_Full_API/wh_full_api_queue.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Global/SoftwareCrc.o ${OBJECTDIR}/Global/global.o ${OBJECTDIR}/Global/provision.o ${OBJECTDIR}/Lib/WirelessHART_Lib.o ${OBJECTDIR}/MS/MS_Data.o ${OBJECTDIR}/MS/MS_Process.o ${OBJECTDIR}/Platform/GlobalVariables.o ${OBJECTDIR}/Platform/gpio.o ${OBJECTDIR}/Platform/interrupt_callbacks.o ${OBJECTDIR}/Platform/measurement.o ${OBJECTDIR}/Platform/nvm.o ${OBJECTDIR}/Platform/timers.o ${OBJECTDIR}/Platform/uart.o ${OBJECTDIR}/Platform/watchdog.o ${OBJECTDIR}/main.o ${OBJECTDIR}/WiredHart/wired_hart_app_layer.o ${OBJECTDIR}/WiredHart/wired_hart_link_layer.o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Global/SoftwareCrc.o.d ${OBJECTDIR}/Global/global.o.d ${OBJECTDIR}/Global/provision.o.d ${OBJECTDIR}/Lib/WirelessHART_Lib.o.d ${OBJECTDIR}/MS/MS_Data.o.d ${OBJECTDIR}/MS/MS_Process.o.d ${OBJECTDIR}/Platform/GlobalVariables.o.d ${OBJECTDIR}/Platform/gpio.o.d ${OBJECTDIR}/Platform/interrupt_callbacks.o.d ${OBJECTDIR}/Platform/measurement.o.d ${OBJECTDIR}/Platform/nvm.o.d ${OBJECTDIR}/Platform/timers.o.d ${OBJECTDIR}/Platform/uart.o.d ${OBJECTDIR}/Platform/watchdog.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/WiredHart/wired_hart_app_layer.o.d ${OBJECTDIR}/WiredHart/wired_hart_link_layer.o.d ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o.d ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o.d ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o.d ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o.d ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o.d ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Global/SoftwareCrc.o ${OBJECTDIR}/Global/global.o ${OBJECTDIR}/Global/provision.o ${OBJECTDIR}/Lib/WirelessHART_Lib.o ${OBJECTDIR}/MS/MS_Data.o ${OBJECTDIR}/MS/MS_Process.o ${OBJECTDIR}/Platform/GlobalVariables.o ${OBJECTDIR}/Platform/gpio.o ${OBJECTDIR}/Platform/interrupt_callbacks.o ${OBJECTDIR}/Platform/measurement.o ${OBJECTDIR}/Platform/nvm.o ${OBJECTDIR}/Platform/timers.o ${OBJECTDIR}/Platform/uart.o ${OBJECTDIR}/Platform/watchdog.o ${OBJECTDIR}/main.o ${OBJECTDIR}/WiredHart/wired_hart_app_layer.o ${OBJECTDIR}/WiredHart/wired_hart_link_layer.o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o

# Source Files
SOURCEFILES=Global/SoftwareCrc.c Global/global.c Global/provision.c Lib/WirelessHART_Lib.c MS/MS_Data.c MS/MS_Process.c Platform/GlobalVariables.c Platform/gpio.c Platform/interrupt_callbacks.c Platform/measurement.c Platform/nvm.c Platform/timers.c Platform/uart.c Platform/watchdog.c main.c WiredHart/wired_hart_app_layer.c WiredHart/wired_hart_link_layer.c WirelessHART_AppLayer/App_WH_CmdCompose.c WirelessHART_AppLayer/App_WH_CmdExecute.c WirelessHART_AppLayer/App_WH_CmdParse.c WirelessHART_AppLayer/App_WH_User.c WirelessHART_Full_API/wh_full_api.c WirelessHART_Full_API/wh_full_api_queue.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/FW-2643-80-04_00.0A.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ256GA406
MP_LINKER_FILE_OPTION=,--script=p24FJ256GA406.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Global/SoftwareCrc.o: Global/SoftwareCrc.c  .generated_files/flags/default/908605ad1694bc04774f15c5a9c06c0518321211 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Global" 
	@${RM} ${OBJECTDIR}/Global/SoftwareCrc.o.d 
	@${RM} ${OBJECTDIR}/Global/SoftwareCrc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Global/SoftwareCrc.c  -o ${OBJECTDIR}/Global/SoftwareCrc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Global/SoftwareCrc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Global/global.o: Global/global.c  .generated_files/flags/default/bac6046034e8e1e6d5ccffa227419ddb4400dde1 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Global" 
	@${RM} ${OBJECTDIR}/Global/global.o.d 
	@${RM} ${OBJECTDIR}/Global/global.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Global/global.c  -o ${OBJECTDIR}/Global/global.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Global/global.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Global/provision.o: Global/provision.c  .generated_files/flags/default/293eb76b6712e86a20157e5844715ff0255a1c93 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Global" 
	@${RM} ${OBJECTDIR}/Global/provision.o.d 
	@${RM} ${OBJECTDIR}/Global/provision.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Global/provision.c  -o ${OBJECTDIR}/Global/provision.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Global/provision.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Lib/WirelessHART_Lib.o: Lib/WirelessHART_Lib.c  .generated_files/flags/default/4ef70b6661f31c3ff6d0d9917e8f7f8269c54d8d .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Lib" 
	@${RM} ${OBJECTDIR}/Lib/WirelessHART_Lib.o.d 
	@${RM} ${OBJECTDIR}/Lib/WirelessHART_Lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Lib/WirelessHART_Lib.c  -o ${OBJECTDIR}/Lib/WirelessHART_Lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Lib/WirelessHART_Lib.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MS/MS_Data.o: MS/MS_Data.c  .generated_files/flags/default/834b947dbd91bba2abed6408442e4a8761089070 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/MS" 
	@${RM} ${OBJECTDIR}/MS/MS_Data.o.d 
	@${RM} ${OBJECTDIR}/MS/MS_Data.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MS/MS_Data.c  -o ${OBJECTDIR}/MS/MS_Data.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MS/MS_Data.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MS/MS_Process.o: MS/MS_Process.c  .generated_files/flags/default/b8eb1ad8232d1b956433897b78060ba468f489a0 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/MS" 
	@${RM} ${OBJECTDIR}/MS/MS_Process.o.d 
	@${RM} ${OBJECTDIR}/MS/MS_Process.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MS/MS_Process.c  -o ${OBJECTDIR}/MS/MS_Process.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MS/MS_Process.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/GlobalVariables.o: Platform/GlobalVariables.c  .generated_files/flags/default/6d02c8801bdc0a270ffc485202c2f3c09e8dd1a5 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/GlobalVariables.o.d 
	@${RM} ${OBJECTDIR}/Platform/GlobalVariables.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/GlobalVariables.c  -o ${OBJECTDIR}/Platform/GlobalVariables.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/GlobalVariables.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/gpio.o: Platform/gpio.c  .generated_files/flags/default/75e66cc11f19731bfa96e1b534375b663309856d .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/gpio.o.d 
	@${RM} ${OBJECTDIR}/Platform/gpio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/gpio.c  -o ${OBJECTDIR}/Platform/gpio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/gpio.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/interrupt_callbacks.o: Platform/interrupt_callbacks.c  .generated_files/flags/default/c50005e8b265472902d19f684102f128853cc448 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/interrupt_callbacks.o.d 
	@${RM} ${OBJECTDIR}/Platform/interrupt_callbacks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/interrupt_callbacks.c  -o ${OBJECTDIR}/Platform/interrupt_callbacks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/interrupt_callbacks.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/measurement.o: Platform/measurement.c  .generated_files/flags/default/37b2160ad5fb02ccba17b088a6f8b589408d6bfd .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/measurement.o.d 
	@${RM} ${OBJECTDIR}/Platform/measurement.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/measurement.c  -o ${OBJECTDIR}/Platform/measurement.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/measurement.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/nvm.o: Platform/nvm.c  .generated_files/flags/default/69ebd6f621d0ed6289a599d944cd83fd8969687e .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/nvm.o.d 
	@${RM} ${OBJECTDIR}/Platform/nvm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/nvm.c  -o ${OBJECTDIR}/Platform/nvm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/nvm.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/timers.o: Platform/timers.c  .generated_files/flags/default/93344667a0462a75a6944ae007f3836f9bb395a3 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/timers.o.d 
	@${RM} ${OBJECTDIR}/Platform/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/timers.c  -o ${OBJECTDIR}/Platform/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/timers.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/uart.o: Platform/uart.c  .generated_files/flags/default/a10b33c055779bcdfa919806a83a4f3f1c28bc .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/uart.o.d 
	@${RM} ${OBJECTDIR}/Platform/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/uart.c  -o ${OBJECTDIR}/Platform/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/uart.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/watchdog.o: Platform/watchdog.c  .generated_files/flags/default/f1c5e7a62289d0b425866fabc424c4ca6182ce51 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/watchdog.o.d 
	@${RM} ${OBJECTDIR}/Platform/watchdog.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/watchdog.c  -o ${OBJECTDIR}/Platform/watchdog.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/watchdog.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/4ad6ac7168b66e0de13203ffb8218eb0737c1288 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WiredHart/wired_hart_app_layer.o: WiredHart/wired_hart_app_layer.c  .generated_files/flags/default/831f25c6eabedad6fdff5cc82588db9a2caab756 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WiredHart" 
	@${RM} ${OBJECTDIR}/WiredHart/wired_hart_app_layer.o.d 
	@${RM} ${OBJECTDIR}/WiredHart/wired_hart_app_layer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WiredHart/wired_hart_app_layer.c  -o ${OBJECTDIR}/WiredHart/wired_hart_app_layer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WiredHart/wired_hart_app_layer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WiredHart/wired_hart_link_layer.o: WiredHart/wired_hart_link_layer.c  .generated_files/flags/default/5d5399b7083718e1ab15785fb4110f1d1e6df66a .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WiredHart" 
	@${RM} ${OBJECTDIR}/WiredHart/wired_hart_link_layer.o.d 
	@${RM} ${OBJECTDIR}/WiredHart/wired_hart_link_layer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WiredHart/wired_hart_link_layer.c  -o ${OBJECTDIR}/WiredHart/wired_hart_link_layer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WiredHart/wired_hart_link_layer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o: WirelessHART_AppLayer/App_WH_CmdCompose.c  .generated_files/flags/default/d45106de98b293231f7c20569a0fbe65f4503e4c .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_AppLayer" 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_AppLayer/App_WH_CmdCompose.c  -o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o: WirelessHART_AppLayer/App_WH_CmdExecute.c  .generated_files/flags/default/e36d3dfe134e3d4df58788775d8c4f00c34f2804 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_AppLayer" 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_AppLayer/App_WH_CmdExecute.c  -o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o: WirelessHART_AppLayer/App_WH_CmdParse.c  .generated_files/flags/default/b3667723a1f6a1d73a2711185ab1595075012fe5 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_AppLayer" 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_AppLayer/App_WH_CmdParse.c  -o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o: WirelessHART_AppLayer/App_WH_User.c  .generated_files/flags/default/742587dc7b29fee09aa72d3c6899a6e05863ef37 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_AppLayer" 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_AppLayer/App_WH_User.c  -o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o: WirelessHART_Full_API/wh_full_api.c  .generated_files/flags/default/211dbeb75669a83c2ad374ced8130fa4ac0c35a3 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_Full_API" 
	@${RM} ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_Full_API/wh_full_api.c  -o ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o: WirelessHART_Full_API/wh_full_api_queue.c  .generated_files/flags/default/ec747a0b7f843f27941dbae75057a4c6d1e8ba89 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_Full_API" 
	@${RM} ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_Full_API/wh_full_api_queue.c  -o ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/Global/SoftwareCrc.o: Global/SoftwareCrc.c  .generated_files/flags/default/578eda4bad92ccf01ab5a009af94aaafb11f722a .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Global" 
	@${RM} ${OBJECTDIR}/Global/SoftwareCrc.o.d 
	@${RM} ${OBJECTDIR}/Global/SoftwareCrc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Global/SoftwareCrc.c  -o ${OBJECTDIR}/Global/SoftwareCrc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Global/SoftwareCrc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Global/global.o: Global/global.c  .generated_files/flags/default/688fb5da29fac08c9183a01d65d4e49d7bd33194 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Global" 
	@${RM} ${OBJECTDIR}/Global/global.o.d 
	@${RM} ${OBJECTDIR}/Global/global.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Global/global.c  -o ${OBJECTDIR}/Global/global.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Global/global.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Global/provision.o: Global/provision.c  .generated_files/flags/default/af66e5feac40fe3c79549e78ae69cab5dfc465a0 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Global" 
	@${RM} ${OBJECTDIR}/Global/provision.o.d 
	@${RM} ${OBJECTDIR}/Global/provision.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Global/provision.c  -o ${OBJECTDIR}/Global/provision.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Global/provision.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Lib/WirelessHART_Lib.o: Lib/WirelessHART_Lib.c  .generated_files/flags/default/bafe142a4ea509c459004a4beb02bb2dedd6647b .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Lib" 
	@${RM} ${OBJECTDIR}/Lib/WirelessHART_Lib.o.d 
	@${RM} ${OBJECTDIR}/Lib/WirelessHART_Lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Lib/WirelessHART_Lib.c  -o ${OBJECTDIR}/Lib/WirelessHART_Lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Lib/WirelessHART_Lib.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MS/MS_Data.o: MS/MS_Data.c  .generated_files/flags/default/f0155ce43d2c97ebe9b864def7b8fe58a6a2ac0 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/MS" 
	@${RM} ${OBJECTDIR}/MS/MS_Data.o.d 
	@${RM} ${OBJECTDIR}/MS/MS_Data.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MS/MS_Data.c  -o ${OBJECTDIR}/MS/MS_Data.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MS/MS_Data.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MS/MS_Process.o: MS/MS_Process.c  .generated_files/flags/default/fee0d57c1c165a6aa0aa9dcb12df5c231a8ec8c2 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/MS" 
	@${RM} ${OBJECTDIR}/MS/MS_Process.o.d 
	@${RM} ${OBJECTDIR}/MS/MS_Process.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MS/MS_Process.c  -o ${OBJECTDIR}/MS/MS_Process.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MS/MS_Process.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/GlobalVariables.o: Platform/GlobalVariables.c  .generated_files/flags/default/9f6152f803a22fe5945da0a1285ba157482a6ceb .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/GlobalVariables.o.d 
	@${RM} ${OBJECTDIR}/Platform/GlobalVariables.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/GlobalVariables.c  -o ${OBJECTDIR}/Platform/GlobalVariables.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/GlobalVariables.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/gpio.o: Platform/gpio.c  .generated_files/flags/default/c12b10af81db1bc20eb7cc271ae9926714d30e5a .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/gpio.o.d 
	@${RM} ${OBJECTDIR}/Platform/gpio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/gpio.c  -o ${OBJECTDIR}/Platform/gpio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/gpio.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/interrupt_callbacks.o: Platform/interrupt_callbacks.c  .generated_files/flags/default/d7b78aeee1df493d66a9312e3d145de901382898 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/interrupt_callbacks.o.d 
	@${RM} ${OBJECTDIR}/Platform/interrupt_callbacks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/interrupt_callbacks.c  -o ${OBJECTDIR}/Platform/interrupt_callbacks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/interrupt_callbacks.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/measurement.o: Platform/measurement.c  .generated_files/flags/default/5fb49b718c2770e46ae789c206cdb4ce6bf6ac5b .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/measurement.o.d 
	@${RM} ${OBJECTDIR}/Platform/measurement.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/measurement.c  -o ${OBJECTDIR}/Platform/measurement.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/measurement.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/nvm.o: Platform/nvm.c  .generated_files/flags/default/faa177eea20532e5c5f80fbda00ee4e826fa8636 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/nvm.o.d 
	@${RM} ${OBJECTDIR}/Platform/nvm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/nvm.c  -o ${OBJECTDIR}/Platform/nvm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/nvm.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/timers.o: Platform/timers.c  .generated_files/flags/default/af1073141b1739f6a0a22d468bf63d8b075dd479 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/timers.o.d 
	@${RM} ${OBJECTDIR}/Platform/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/timers.c  -o ${OBJECTDIR}/Platform/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/timers.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/uart.o: Platform/uart.c  .generated_files/flags/default/f848d6ee1b96bc1f89833f53a7994d71813f37a5 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/uart.o.d 
	@${RM} ${OBJECTDIR}/Platform/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/uart.c  -o ${OBJECTDIR}/Platform/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/uart.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/watchdog.o: Platform/watchdog.c  .generated_files/flags/default/9030b83e942acf0d54620c0e5d4434e7f3709ce7 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/watchdog.o.d 
	@${RM} ${OBJECTDIR}/Platform/watchdog.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/watchdog.c  -o ${OBJECTDIR}/Platform/watchdog.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/watchdog.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/d89a811a343d4028762483a3ed940e276668c5bb .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WiredHart/wired_hart_app_layer.o: WiredHart/wired_hart_app_layer.c  .generated_files/flags/default/2308e62009e885743d8b6b9c323c4b509a155564 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WiredHart" 
	@${RM} ${OBJECTDIR}/WiredHart/wired_hart_app_layer.o.d 
	@${RM} ${OBJECTDIR}/WiredHart/wired_hart_app_layer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WiredHart/wired_hart_app_layer.c  -o ${OBJECTDIR}/WiredHart/wired_hart_app_layer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WiredHart/wired_hart_app_layer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WiredHart/wired_hart_link_layer.o: WiredHart/wired_hart_link_layer.c  .generated_files/flags/default/ded43fdaf9c43bd1c81a0bd67047b8ca373e11a8 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WiredHart" 
	@${RM} ${OBJECTDIR}/WiredHart/wired_hart_link_layer.o.d 
	@${RM} ${OBJECTDIR}/WiredHart/wired_hart_link_layer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WiredHart/wired_hart_link_layer.c  -o ${OBJECTDIR}/WiredHart/wired_hart_link_layer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WiredHart/wired_hart_link_layer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o: WirelessHART_AppLayer/App_WH_CmdCompose.c  .generated_files/flags/default/eaaee86afdc6485c290ea704e6ca07c048efb385 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_AppLayer" 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_AppLayer/App_WH_CmdCompose.c  -o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o: WirelessHART_AppLayer/App_WH_CmdExecute.c  .generated_files/flags/default/61388b43e4c1a049674c1a562d7b83def6d4fbf3 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_AppLayer" 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_AppLayer/App_WH_CmdExecute.c  -o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o: WirelessHART_AppLayer/App_WH_CmdParse.c  .generated_files/flags/default/f6be6391d36b76d52ce76d9eb2e7d2326e6b35f5 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_AppLayer" 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_AppLayer/App_WH_CmdParse.c  -o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o: WirelessHART_AppLayer/App_WH_User.c  .generated_files/flags/default/54a4482a6518e5ae92954de350c5ad41fe365299 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_AppLayer" 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_AppLayer/App_WH_User.c  -o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o: WirelessHART_Full_API/wh_full_api.c  .generated_files/flags/default/29eea433d259c1f2863c0ea7da4d839347dbe9a2 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_Full_API" 
	@${RM} ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_Full_API/wh_full_api.c  -o ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o: WirelessHART_Full_API/wh_full_api_queue.c  .generated_files/flags/default/86b2d3d76111ce4da2067c2e8d26fab25ea41152 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_Full_API" 
	@${RM} ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_Full_API/wh_full_api_queue.c  -o ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/FW-2643-80-04_00.0A.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/FW-2643-80-04_00.0A.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81B -mreserve=data@0x81C:0x81D -mreserve=data@0x81E:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x827 -mreserve=data@0x82A:0x84F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=1000,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/FW-2643-80-04_00.0A.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/FW-2643-80-04_00.0A.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=1000,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/FW-2643-80-04_00.0A.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
