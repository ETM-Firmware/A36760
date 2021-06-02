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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/A34760.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/A34760.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../Main.c ../LTC2656.c ../ETMI2C.c ../ETMSPI.c ../MCP23017.c ../IOPorts.c ../ETMdsp.s ../A34760.c ../faults.c ../Serial.c ../Buffer64.c ../ETM_SCALE.s ../eeprom.c ../ETM_CRC.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/Main.o ${OBJECTDIR}/_ext/1472/LTC2656.o ${OBJECTDIR}/_ext/1472/ETMI2C.o ${OBJECTDIR}/_ext/1472/ETMSPI.o ${OBJECTDIR}/_ext/1472/MCP23017.o ${OBJECTDIR}/_ext/1472/IOPorts.o ${OBJECTDIR}/_ext/1472/ETMdsp.o ${OBJECTDIR}/_ext/1472/A34760.o ${OBJECTDIR}/_ext/1472/faults.o ${OBJECTDIR}/_ext/1472/Serial.o ${OBJECTDIR}/_ext/1472/Buffer64.o ${OBJECTDIR}/_ext/1472/ETM_SCALE.o ${OBJECTDIR}/_ext/1472/eeprom.o ${OBJECTDIR}/_ext/1472/ETM_CRC.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/Main.o.d ${OBJECTDIR}/_ext/1472/LTC2656.o.d ${OBJECTDIR}/_ext/1472/ETMI2C.o.d ${OBJECTDIR}/_ext/1472/ETMSPI.o.d ${OBJECTDIR}/_ext/1472/MCP23017.o.d ${OBJECTDIR}/_ext/1472/IOPorts.o.d ${OBJECTDIR}/_ext/1472/ETMdsp.o.d ${OBJECTDIR}/_ext/1472/A34760.o.d ${OBJECTDIR}/_ext/1472/faults.o.d ${OBJECTDIR}/_ext/1472/Serial.o.d ${OBJECTDIR}/_ext/1472/Buffer64.o.d ${OBJECTDIR}/_ext/1472/ETM_SCALE.o.d ${OBJECTDIR}/_ext/1472/eeprom.o.d ${OBJECTDIR}/_ext/1472/ETM_CRC.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/Main.o ${OBJECTDIR}/_ext/1472/LTC2656.o ${OBJECTDIR}/_ext/1472/ETMI2C.o ${OBJECTDIR}/_ext/1472/ETMSPI.o ${OBJECTDIR}/_ext/1472/MCP23017.o ${OBJECTDIR}/_ext/1472/IOPorts.o ${OBJECTDIR}/_ext/1472/ETMdsp.o ${OBJECTDIR}/_ext/1472/A34760.o ${OBJECTDIR}/_ext/1472/faults.o ${OBJECTDIR}/_ext/1472/Serial.o ${OBJECTDIR}/_ext/1472/Buffer64.o ${OBJECTDIR}/_ext/1472/ETM_SCALE.o ${OBJECTDIR}/_ext/1472/eeprom.o ${OBJECTDIR}/_ext/1472/ETM_CRC.o

# Source Files
SOURCEFILES=../Main.c ../LTC2656.c ../ETMI2C.c ../ETMSPI.c ../MCP23017.c ../IOPorts.c ../ETMdsp.s ../A34760.c ../faults.c ../Serial.c ../Buffer64.c ../ETM_SCALE.s ../eeprom.c ../ETM_CRC.c



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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/A34760.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=30F6014A
MP_LINKER_FILE_OPTION=,--script=p30F6014A.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/Main.o: ../Main.c  .generated_files/9c5fc1ce548a13487c347e7a4054b99650a3bbd0.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Main.c  -o ${OBJECTDIR}/_ext/1472/Main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/Main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/LTC2656.o: ../LTC2656.c  .generated_files/eab093158f75cba21c34d7c78803224089b7f1cc.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/LTC2656.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/LTC2656.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../LTC2656.c  -o ${OBJECTDIR}/_ext/1472/LTC2656.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/LTC2656.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/ETMI2C.o: ../ETMI2C.c  .generated_files/726adf9886427caa17e9eace6210c9ee0d185f7b.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMI2C.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMI2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../ETMI2C.c  -o ${OBJECTDIR}/_ext/1472/ETMI2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/ETMI2C.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/ETMSPI.o: ../ETMSPI.c  .generated_files/428a427b641fabcf64fe7e890ba61d32b738ccaf.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMSPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMSPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../ETMSPI.c  -o ${OBJECTDIR}/_ext/1472/ETMSPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/ETMSPI.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/MCP23017.o: ../MCP23017.c  .generated_files/1ffac8597d1b64c005783493eb89bc746e46d182.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/MCP23017.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/MCP23017.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../MCP23017.c  -o ${OBJECTDIR}/_ext/1472/MCP23017.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/MCP23017.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/IOPorts.o: ../IOPorts.c  .generated_files/e94ab0ce67ce390f5c02daadc653a29f87337d48.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/IOPorts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/IOPorts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../IOPorts.c  -o ${OBJECTDIR}/_ext/1472/IOPorts.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/IOPorts.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/A34760.o: ../A34760.c  .generated_files/a58408b3b65dcacd73e28fb7c019095d0c40fdf4.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/A34760.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/A34760.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../A34760.c  -o ${OBJECTDIR}/_ext/1472/A34760.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/A34760.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/faults.o: ../faults.c  .generated_files/97d37849d5bb1a1c65376dcc7bd3d79ef469dda3.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/faults.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/faults.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../faults.c  -o ${OBJECTDIR}/_ext/1472/faults.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/faults.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/Serial.o: ../Serial.c  .generated_files/2913887d13caa00f3a01e2deb48578b23cacd33a.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Serial.c  -o ${OBJECTDIR}/_ext/1472/Serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/Serial.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/Buffer64.o: ../Buffer64.c  .generated_files/7df68d51131ea80de14f22caa01379154061e43a.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Buffer64.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Buffer64.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Buffer64.c  -o ${OBJECTDIR}/_ext/1472/Buffer64.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/Buffer64.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/eeprom.o: ../eeprom.c  .generated_files/9acd10c908d6fc1bd9c670fc13ace3b11a4ddc96.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../eeprom.c  -o ${OBJECTDIR}/_ext/1472/eeprom.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/eeprom.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/ETM_CRC.o: ../ETM_CRC.c  .generated_files/2d014ce9da11767ace6c278fa4955350a93869c2.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ETM_CRC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ETM_CRC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../ETM_CRC.c  -o ${OBJECTDIR}/_ext/1472/ETM_CRC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/ETM_CRC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
else
${OBJECTDIR}/_ext/1472/Main.o: ../Main.c  .generated_files/f708fa2b9853310080110dcbad460082bfe61cca.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Main.c  -o ${OBJECTDIR}/_ext/1472/Main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/Main.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/LTC2656.o: ../LTC2656.c  .generated_files/9a821b02d2f610da1a72066b77c8f8a18af33f36.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/LTC2656.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/LTC2656.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../LTC2656.c  -o ${OBJECTDIR}/_ext/1472/LTC2656.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/LTC2656.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/ETMI2C.o: ../ETMI2C.c  .generated_files/dd829d705738bd1b4b3ab10bcc45a1e9fe90db40.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMI2C.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMI2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../ETMI2C.c  -o ${OBJECTDIR}/_ext/1472/ETMI2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/ETMI2C.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/ETMSPI.o: ../ETMSPI.c  .generated_files/81470f6831d09a65a6a711b600a8866e9eb4d908.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMSPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMSPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../ETMSPI.c  -o ${OBJECTDIR}/_ext/1472/ETMSPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/ETMSPI.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/MCP23017.o: ../MCP23017.c  .generated_files/65c43e333c7762cf5ddf5e13b0f6db83a3bf09cf.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/MCP23017.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/MCP23017.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../MCP23017.c  -o ${OBJECTDIR}/_ext/1472/MCP23017.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/MCP23017.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/IOPorts.o: ../IOPorts.c  .generated_files/b0992848188eade7a357af78ad9abef258509b96.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/IOPorts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/IOPorts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../IOPorts.c  -o ${OBJECTDIR}/_ext/1472/IOPorts.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/IOPorts.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/A34760.o: ../A34760.c  .generated_files/12894b67c9153ed6820d285b4dd32fff6968616b.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/A34760.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/A34760.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../A34760.c  -o ${OBJECTDIR}/_ext/1472/A34760.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/A34760.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/faults.o: ../faults.c  .generated_files/739537e95da4d922ac17a7542477e94085ed9f9b.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/faults.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/faults.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../faults.c  -o ${OBJECTDIR}/_ext/1472/faults.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/faults.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/Serial.o: ../Serial.c  .generated_files/573a7ab50c75d1050475a4253664356ea1eeda9d.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Serial.c  -o ${OBJECTDIR}/_ext/1472/Serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/Serial.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/Buffer64.o: ../Buffer64.c  .generated_files/a56154cd4d1ed9eb0c697651a29e74675a0c20c4.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Buffer64.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Buffer64.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Buffer64.c  -o ${OBJECTDIR}/_ext/1472/Buffer64.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/Buffer64.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/eeprom.o: ../eeprom.c  .generated_files/52fdded2bff51fd9b379c35512e63db3dddd3979.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/eeprom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../eeprom.c  -o ${OBJECTDIR}/_ext/1472/eeprom.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/eeprom.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1472/ETM_CRC.o: ../ETM_CRC.c  .generated_files/4b27441b87ad6b79da98b6a86d562cdd32cea5f1.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ETM_CRC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ETM_CRC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../ETM_CRC.c  -o ${OBJECTDIR}/_ext/1472/ETM_CRC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/ETM_CRC.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/ETMdsp.o: ../ETMdsp.s  .generated_files/c150c8b6c96dee266e7d634c4ae78d11aa3d9306.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMdsp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMdsp.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../ETMdsp.s  -o ${OBJECTDIR}/_ext/1472/ETMdsp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -DXPRJ_default=$(CND_CONF)    -I".." -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1472/ETMdsp.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST) 
	
${OBJECTDIR}/_ext/1472/ETM_SCALE.o: ../ETM_SCALE.s  .generated_files/71665083fa1705071a5b40e27ffae0d2982b5f84.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ETM_SCALE.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ETM_SCALE.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../ETM_SCALE.s  -o ${OBJECTDIR}/_ext/1472/ETM_SCALE.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -DXPRJ_default=$(CND_CONF)    -I".." -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1472/ETM_SCALE.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST) 
	
else
${OBJECTDIR}/_ext/1472/ETMdsp.o: ../ETMdsp.s  .generated_files/87bd1d5f45e0e92531881f68b4f47cb2294fe1e6.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMdsp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMdsp.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../ETMdsp.s  -o ${OBJECTDIR}/_ext/1472/ETMdsp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_default=$(CND_CONF)    -I".." -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1472/ETMdsp.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST) 
	
${OBJECTDIR}/_ext/1472/ETM_SCALE.o: ../ETM_SCALE.s  .generated_files/4f7b7ecb1ab3221c32ee866c592f66d60ee7c90f.flag .generated_files/73b1a2a3aa14fa01b22a88d416ed5ddb6862e04.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ETM_SCALE.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ETM_SCALE.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../ETM_SCALE.s  -o ${OBJECTDIR}/_ext/1472/ETM_SCALE.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_default=$(CND_CONF)    -I".." -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1472/ETM_SCALE.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/A34760.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  C:/Program\ Files\ (x86)/Microchip/xc16/v1.23/lib/libdsp-elf.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/A34760.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    "C:\Program Files (x86)\Microchip\xc16\v1.23\lib\libdsp-elf.a"  -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x84F   -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--library-path=".",--no-force-link,--smart-io,-Map="${DISTDIR}/A34760.X.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST)  
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/A34760.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  C:/Program\ Files\ (x86)/Microchip/xc16/v1.23/lib/libdsp-elf.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/A34760.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    "C:\Program Files (x86)\Microchip\xc16\v1.23\lib\libdsp-elf.a"  -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--library-path=".",--no-force-link,--smart-io,-Map="${DISTDIR}/A34760.X.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST)  
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/A34760.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
