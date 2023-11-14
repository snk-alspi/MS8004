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
FINAL_IMAGE=${DISTDIR}/FW-2643-80-04_00.0B.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/FW-2643-80-04_00.0B.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/FW-2643-80-04_00.0B.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ256GA406
MP_LINKER_FILE_OPTION=,--script=p24FJ256GA406.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Global/SoftwareCrc.o: Global/SoftwareCrc.c  .generated_files/flags/default/ca7ed0f1202e90dc5864cf6c3d42472d9615566a .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Global" 
	@${RM} ${OBJECTDIR}/Global/SoftwareCrc.o.d 
	@${RM} ${OBJECTDIR}/Global/SoftwareCrc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Global/SoftwareCrc.c  -o ${OBJECTDIR}/Global/SoftwareCrc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Global/SoftwareCrc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Global/global.o: Global/global.c  .generated_files/flags/default/4c870437dd0d22899cc445f6ade5e4d17efc3461 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Global" 
	@${RM} ${OBJECTDIR}/Global/global.o.d 
	@${RM} ${OBJECTDIR}/Global/global.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Global/global.c  -o ${OBJECTDIR}/Global/global.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Global/global.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Global/provision.o: Global/provision.c  .generated_files/flags/default/1cc072e4327a4dc8a6aee0cdb519f7166b6f25a3 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Global" 
	@${RM} ${OBJECTDIR}/Global/provision.o.d 
	@${RM} ${OBJECTDIR}/Global/provision.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Global/provision.c  -o ${OBJECTDIR}/Global/provision.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Global/provision.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Lib/WirelessHART_Lib.o: Lib/WirelessHART_Lib.c  .generated_files/flags/default/5b3e1e69220cbb4ce1bc5eaffebfa4378d45d9ed .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Lib" 
	@${RM} ${OBJECTDIR}/Lib/WirelessHART_Lib.o.d 
	@${RM} ${OBJECTDIR}/Lib/WirelessHART_Lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Lib/WirelessHART_Lib.c  -o ${OBJECTDIR}/Lib/WirelessHART_Lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Lib/WirelessHART_Lib.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MS/MS_Data.o: MS/MS_Data.c  .generated_files/flags/default/e3e89c8f81551681924b07ce0f41a83a2c864eae .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/MS" 
	@${RM} ${OBJECTDIR}/MS/MS_Data.o.d 
	@${RM} ${OBJECTDIR}/MS/MS_Data.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MS/MS_Data.c  -o ${OBJECTDIR}/MS/MS_Data.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MS/MS_Data.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MS/MS_Process.o: MS/MS_Process.c  .generated_files/flags/default/d1174ab22a548e536f215811d8ccf212c71e5658 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/MS" 
	@${RM} ${OBJECTDIR}/MS/MS_Process.o.d 
	@${RM} ${OBJECTDIR}/MS/MS_Process.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MS/MS_Process.c  -o ${OBJECTDIR}/MS/MS_Process.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MS/MS_Process.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/GlobalVariables.o: Platform/GlobalVariables.c  .generated_files/flags/default/b05469142a1b6c81a2a447015cb0d1b555ff90cb .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/GlobalVariables.o.d 
	@${RM} ${OBJECTDIR}/Platform/GlobalVariables.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/GlobalVariables.c  -o ${OBJECTDIR}/Platform/GlobalVariables.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/GlobalVariables.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/gpio.o: Platform/gpio.c  .generated_files/flags/default/367f37417640752922063d56d0f18e5853cc3333 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/gpio.o.d 
	@${RM} ${OBJECTDIR}/Platform/gpio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/gpio.c  -o ${OBJECTDIR}/Platform/gpio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/gpio.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/interrupt_callbacks.o: Platform/interrupt_callbacks.c  .generated_files/flags/default/37669361726f726f15d124e1c86d1a09d1e6d3b4 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/interrupt_callbacks.o.d 
	@${RM} ${OBJECTDIR}/Platform/interrupt_callbacks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/interrupt_callbacks.c  -o ${OBJECTDIR}/Platform/interrupt_callbacks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/interrupt_callbacks.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/measurement.o: Platform/measurement.c  .generated_files/flags/default/804a6f7c3d2ccab740ca074fb159e88ff3056a47 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/measurement.o.d 
	@${RM} ${OBJECTDIR}/Platform/measurement.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/measurement.c  -o ${OBJECTDIR}/Platform/measurement.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/measurement.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/nvm.o: Platform/nvm.c  .generated_files/flags/default/a4fbfb03ec1fbd650699c8989f1913abf4a41af3 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/nvm.o.d 
	@${RM} ${OBJECTDIR}/Platform/nvm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/nvm.c  -o ${OBJECTDIR}/Platform/nvm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/nvm.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/timers.o: Platform/timers.c  .generated_files/flags/default/aad677ebdcf67e03c6ae16835456e25a0ae7565 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/timers.o.d 
	@${RM} ${OBJECTDIR}/Platform/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/timers.c  -o ${OBJECTDIR}/Platform/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/timers.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/uart.o: Platform/uart.c  .generated_files/flags/default/b2618266cd0b44f1bdb2a881b9a914ca45bbbc30 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/uart.o.d 
	@${RM} ${OBJECTDIR}/Platform/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/uart.c  -o ${OBJECTDIR}/Platform/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/uart.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/watchdog.o: Platform/watchdog.c  .generated_files/flags/default/c530e6bbd49a93f5e01689be03d8bf2c46176c39 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/watchdog.o.d 
	@${RM} ${OBJECTDIR}/Platform/watchdog.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/watchdog.c  -o ${OBJECTDIR}/Platform/watchdog.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/watchdog.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/790eb607fd78a8195841eefe7af188746a88597e .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WiredHart/wired_hart_app_layer.o: WiredHart/wired_hart_app_layer.c  .generated_files/flags/default/dafaa1b4ec7262896ae182cc0ceb83b22f8df64f .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WiredHart" 
	@${RM} ${OBJECTDIR}/WiredHart/wired_hart_app_layer.o.d 
	@${RM} ${OBJECTDIR}/WiredHart/wired_hart_app_layer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WiredHart/wired_hart_app_layer.c  -o ${OBJECTDIR}/WiredHart/wired_hart_app_layer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WiredHart/wired_hart_app_layer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WiredHart/wired_hart_link_layer.o: WiredHart/wired_hart_link_layer.c  .generated_files/flags/default/94f12b20368fc7d77cf1f229508bed95c7ab0456 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WiredHart" 
	@${RM} ${OBJECTDIR}/WiredHart/wired_hart_link_layer.o.d 
	@${RM} ${OBJECTDIR}/WiredHart/wired_hart_link_layer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WiredHart/wired_hart_link_layer.c  -o ${OBJECTDIR}/WiredHart/wired_hart_link_layer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WiredHart/wired_hart_link_layer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o: WirelessHART_AppLayer/App_WH_CmdCompose.c  .generated_files/flags/default/e06845ec55184f8bee5962a2f87cdd4607173e7a .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_AppLayer" 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_AppLayer/App_WH_CmdCompose.c  -o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o: WirelessHART_AppLayer/App_WH_CmdExecute.c  .generated_files/flags/default/7a79855ceef151e90c76b1c5ccce39a1e5050317 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_AppLayer" 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_AppLayer/App_WH_CmdExecute.c  -o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o: WirelessHART_AppLayer/App_WH_CmdParse.c  .generated_files/flags/default/9d863f3c924f1cc3f1dff84a572985c37b70d6b9 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_AppLayer" 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_AppLayer/App_WH_CmdParse.c  -o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o: WirelessHART_AppLayer/App_WH_User.c  .generated_files/flags/default/4b1170aa77df5552dd50c36b63cff265cdf42842 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_AppLayer" 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_AppLayer/App_WH_User.c  -o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o: WirelessHART_Full_API/wh_full_api.c  .generated_files/flags/default/791ec711f62db97de341f782a1a9ea936706bbda .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_Full_API" 
	@${RM} ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_Full_API/wh_full_api.c  -o ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o: WirelessHART_Full_API/wh_full_api_queue.c  .generated_files/flags/default/9fba28b6b9683403950ee51298433aede6ced58d .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_Full_API" 
	@${RM} ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_Full_API/wh_full_api_queue.c  -o ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/Global/SoftwareCrc.o: Global/SoftwareCrc.c  .generated_files/flags/default/5a3f75e30c7d4b3f748c6b41a9afcdd0d0cf5dd4 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Global" 
	@${RM} ${OBJECTDIR}/Global/SoftwareCrc.o.d 
	@${RM} ${OBJECTDIR}/Global/SoftwareCrc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Global/SoftwareCrc.c  -o ${OBJECTDIR}/Global/SoftwareCrc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Global/SoftwareCrc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Global/global.o: Global/global.c  .generated_files/flags/default/2eaa940312174919166b09ee50f15c479e42ca6b .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Global" 
	@${RM} ${OBJECTDIR}/Global/global.o.d 
	@${RM} ${OBJECTDIR}/Global/global.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Global/global.c  -o ${OBJECTDIR}/Global/global.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Global/global.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Global/provision.o: Global/provision.c  .generated_files/flags/default/e38462d3d3770a39b6bf0ff51fe84408b391611d .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Global" 
	@${RM} ${OBJECTDIR}/Global/provision.o.d 
	@${RM} ${OBJECTDIR}/Global/provision.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Global/provision.c  -o ${OBJECTDIR}/Global/provision.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Global/provision.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Lib/WirelessHART_Lib.o: Lib/WirelessHART_Lib.c  .generated_files/flags/default/8173e1ef9fcb7ab5eff94be5b354b1639a635020 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Lib" 
	@${RM} ${OBJECTDIR}/Lib/WirelessHART_Lib.o.d 
	@${RM} ${OBJECTDIR}/Lib/WirelessHART_Lib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Lib/WirelessHART_Lib.c  -o ${OBJECTDIR}/Lib/WirelessHART_Lib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Lib/WirelessHART_Lib.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MS/MS_Data.o: MS/MS_Data.c  .generated_files/flags/default/cd03f64e10accf849611fe6e20023c5f7a71ec16 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/MS" 
	@${RM} ${OBJECTDIR}/MS/MS_Data.o.d 
	@${RM} ${OBJECTDIR}/MS/MS_Data.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MS/MS_Data.c  -o ${OBJECTDIR}/MS/MS_Data.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MS/MS_Data.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/MS/MS_Process.o: MS/MS_Process.c  .generated_files/flags/default/e65edaad627692016e913352d797626bca707ad7 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/MS" 
	@${RM} ${OBJECTDIR}/MS/MS_Process.o.d 
	@${RM} ${OBJECTDIR}/MS/MS_Process.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  MS/MS_Process.c  -o ${OBJECTDIR}/MS/MS_Process.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/MS/MS_Process.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/GlobalVariables.o: Platform/GlobalVariables.c  .generated_files/flags/default/c348169938f623e1370b2fd6aeecd78dd3264b3c .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/GlobalVariables.o.d 
	@${RM} ${OBJECTDIR}/Platform/GlobalVariables.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/GlobalVariables.c  -o ${OBJECTDIR}/Platform/GlobalVariables.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/GlobalVariables.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/gpio.o: Platform/gpio.c  .generated_files/flags/default/23832806041632c3b0b19d452879817cdf2dfef9 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/gpio.o.d 
	@${RM} ${OBJECTDIR}/Platform/gpio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/gpio.c  -o ${OBJECTDIR}/Platform/gpio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/gpio.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/interrupt_callbacks.o: Platform/interrupt_callbacks.c  .generated_files/flags/default/ef835723fc7bda9ccb112fc83fde65c5c6a8f2cc .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/interrupt_callbacks.o.d 
	@${RM} ${OBJECTDIR}/Platform/interrupt_callbacks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/interrupt_callbacks.c  -o ${OBJECTDIR}/Platform/interrupt_callbacks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/interrupt_callbacks.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/measurement.o: Platform/measurement.c  .generated_files/flags/default/5195e361486c9c7d10580a4bf21524e37a8e1e1c .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/measurement.o.d 
	@${RM} ${OBJECTDIR}/Platform/measurement.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/measurement.c  -o ${OBJECTDIR}/Platform/measurement.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/measurement.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/nvm.o: Platform/nvm.c  .generated_files/flags/default/2b8ff1006029726629c8311f5521004fb65de4c7 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/nvm.o.d 
	@${RM} ${OBJECTDIR}/Platform/nvm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/nvm.c  -o ${OBJECTDIR}/Platform/nvm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/nvm.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/timers.o: Platform/timers.c  .generated_files/flags/default/9a2f095ccd768b45113e009d10746210888abffb .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/timers.o.d 
	@${RM} ${OBJECTDIR}/Platform/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/timers.c  -o ${OBJECTDIR}/Platform/timers.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/timers.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/uart.o: Platform/uart.c  .generated_files/flags/default/44f61719d2dc8b81936b57609388aab896ef070a .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/uart.o.d 
	@${RM} ${OBJECTDIR}/Platform/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/uart.c  -o ${OBJECTDIR}/Platform/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/uart.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Platform/watchdog.o: Platform/watchdog.c  .generated_files/flags/default/1747a0b2e9d6fbc9697714951e5950d64404b4d5 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/Platform" 
	@${RM} ${OBJECTDIR}/Platform/watchdog.o.d 
	@${RM} ${OBJECTDIR}/Platform/watchdog.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Platform/watchdog.c  -o ${OBJECTDIR}/Platform/watchdog.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Platform/watchdog.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/aecb771de24a14e4f28fe9728157eea57b0802f7 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WiredHart/wired_hart_app_layer.o: WiredHart/wired_hart_app_layer.c  .generated_files/flags/default/ec495e4ea2994cb7fc4df0815930b890760b3745 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WiredHart" 
	@${RM} ${OBJECTDIR}/WiredHart/wired_hart_app_layer.o.d 
	@${RM} ${OBJECTDIR}/WiredHart/wired_hart_app_layer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WiredHart/wired_hart_app_layer.c  -o ${OBJECTDIR}/WiredHart/wired_hart_app_layer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WiredHart/wired_hart_app_layer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WiredHart/wired_hart_link_layer.o: WiredHart/wired_hart_link_layer.c  .generated_files/flags/default/175c9e72dc05b44cfc45621c6b2c38f392e6e6f5 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WiredHart" 
	@${RM} ${OBJECTDIR}/WiredHart/wired_hart_link_layer.o.d 
	@${RM} ${OBJECTDIR}/WiredHart/wired_hart_link_layer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WiredHart/wired_hart_link_layer.c  -o ${OBJECTDIR}/WiredHart/wired_hart_link_layer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WiredHart/wired_hart_link_layer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o: WirelessHART_AppLayer/App_WH_CmdCompose.c  .generated_files/flags/default/bd46ddb0e5c6b9d5bf86131b69f2f9e94d726cd5 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_AppLayer" 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_AppLayer/App_WH_CmdCompose.c  -o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdCompose.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o: WirelessHART_AppLayer/App_WH_CmdExecute.c  .generated_files/flags/default/73a9012de6397590b248421c95849680f59eece .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_AppLayer" 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_AppLayer/App_WH_CmdExecute.c  -o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdExecute.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o: WirelessHART_AppLayer/App_WH_CmdParse.c  .generated_files/flags/default/16f61399839c072d4bee072566c6c8e3c636e8e .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_AppLayer" 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_AppLayer/App_WH_CmdParse.c  -o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_AppLayer/App_WH_CmdParse.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o: WirelessHART_AppLayer/App_WH_User.c  .generated_files/flags/default/cfb11fffc472375f564b2826a06b2c9a8f1c908f .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_AppLayer" 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_AppLayer/App_WH_User.c  -o ${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_AppLayer/App_WH_User.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o: WirelessHART_Full_API/wh_full_api.c  .generated_files/flags/default/72cfc5c81250dbd81b98e73fbc77afae0fb21d5a .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
	@${MKDIR} "${OBJECTDIR}/WirelessHART_Full_API" 
	@${RM} ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o.d 
	@${RM} ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  WirelessHART_Full_API/wh_full_api.c  -o ${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/WirelessHART_Full_API/wh_full_api.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -mlarge-scalar -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/WirelessHART_Full_API/wh_full_api_queue.o: WirelessHART_Full_API/wh_full_api_queue.c  .generated_files/flags/default/d3173dee1a9b92d050e15bfb6c195224009054c4 .generated_files/flags/default/7a173805e7e56b2bd3cd2b95b46e6e4a0c492a58
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
${DISTDIR}/FW-2643-80-04_00.0B.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/FW-2643-80-04_00.0B.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81B -mreserve=data@0x81C:0x81D -mreserve=data@0x81E:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x827 -mreserve=data@0x82A:0x84F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=1000,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/FW-2643-80-04_00.0B.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/FW-2643-80-04_00.0B.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=1000,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/FW-2643-80-04_00.0B.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
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
