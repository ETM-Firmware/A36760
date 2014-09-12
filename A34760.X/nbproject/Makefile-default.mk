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

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../Main.c ../LTC2656.c ../ETMI2C.c ../ETMSPI.c ../MCP23017.c ../IOPorts.c ../ETMdsp.s ../A34760.c ../faults.c ../Serial.c ../Buffer64.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/Main.o ${OBJECTDIR}/_ext/1472/LTC2656.o ${OBJECTDIR}/_ext/1472/ETMI2C.o ${OBJECTDIR}/_ext/1472/ETMSPI.o ${OBJECTDIR}/_ext/1472/MCP23017.o ${OBJECTDIR}/_ext/1472/IOPorts.o ${OBJECTDIR}/_ext/1472/ETMdsp.o ${OBJECTDIR}/_ext/1472/A34760.o ${OBJECTDIR}/_ext/1472/faults.o ${OBJECTDIR}/_ext/1472/Serial.o ${OBJECTDIR}/_ext/1472/Buffer64.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/Main.o.d ${OBJECTDIR}/_ext/1472/LTC2656.o.d ${OBJECTDIR}/_ext/1472/ETMI2C.o.d ${OBJECTDIR}/_ext/1472/ETMSPI.o.d ${OBJECTDIR}/_ext/1472/MCP23017.o.d ${OBJECTDIR}/_ext/1472/IOPorts.o.d ${OBJECTDIR}/_ext/1472/ETMdsp.o.d ${OBJECTDIR}/_ext/1472/A34760.o.d ${OBJECTDIR}/_ext/1472/faults.o.d ${OBJECTDIR}/_ext/1472/Serial.o.d ${OBJECTDIR}/_ext/1472/Buffer64.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/Main.o ${OBJECTDIR}/_ext/1472/LTC2656.o ${OBJECTDIR}/_ext/1472/ETMI2C.o ${OBJECTDIR}/_ext/1472/ETMSPI.o ${OBJECTDIR}/_ext/1472/MCP23017.o ${OBJECTDIR}/_ext/1472/IOPorts.o ${OBJECTDIR}/_ext/1472/ETMdsp.o ${OBJECTDIR}/_ext/1472/A34760.o ${OBJECTDIR}/_ext/1472/faults.o ${OBJECTDIR}/_ext/1472/Serial.o ${OBJECTDIR}/_ext/1472/Buffer64.o

# Source Files
SOURCEFILES=../Main.c ../LTC2656.c ../ETMI2C.c ../ETMSPI.c ../MCP23017.c ../IOPorts.c ../ETMdsp.s ../A34760.c ../faults.c ../Serial.c ../Buffer64.c


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
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/A34760.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=30F6014A
MP_LINKER_FILE_OPTION=,--script=p30F6014A.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/Main.o: ../Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Main.c  -o ${OBJECTDIR}/_ext/1472/Main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/Main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/LTC2656.o: ../LTC2656.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/LTC2656.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/LTC2656.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../LTC2656.c  -o ${OBJECTDIR}/_ext/1472/LTC2656.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/LTC2656.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/LTC2656.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/ETMI2C.o: ../ETMI2C.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMI2C.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMI2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../ETMI2C.c  -o ${OBJECTDIR}/_ext/1472/ETMI2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/ETMI2C.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/ETMI2C.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/ETMSPI.o: ../ETMSPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMSPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMSPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../ETMSPI.c  -o ${OBJECTDIR}/_ext/1472/ETMSPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/ETMSPI.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/ETMSPI.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/MCP23017.o: ../MCP23017.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/MCP23017.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/MCP23017.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../MCP23017.c  -o ${OBJECTDIR}/_ext/1472/MCP23017.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/MCP23017.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/MCP23017.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/IOPorts.o: ../IOPorts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/IOPorts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/IOPorts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../IOPorts.c  -o ${OBJECTDIR}/_ext/1472/IOPorts.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/IOPorts.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/IOPorts.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/A34760.o: ../A34760.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/A34760.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/A34760.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../A34760.c  -o ${OBJECTDIR}/_ext/1472/A34760.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/A34760.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/A34760.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/faults.o: ../faults.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/faults.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/faults.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../faults.c  -o ${OBJECTDIR}/_ext/1472/faults.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/faults.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/faults.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/Serial.o: ../Serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Serial.c  -o ${OBJECTDIR}/_ext/1472/Serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/Serial.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Serial.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/Buffer64.o: ../Buffer64.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Buffer64.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Buffer64.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Buffer64.c  -o ${OBJECTDIR}/_ext/1472/Buffer64.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/Buffer64.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Buffer64.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1472/Main.o: ../Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Main.c  -o ${OBJECTDIR}/_ext/1472/Main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/Main.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/LTC2656.o: ../LTC2656.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/LTC2656.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/LTC2656.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../LTC2656.c  -o ${OBJECTDIR}/_ext/1472/LTC2656.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/LTC2656.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/LTC2656.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/ETMI2C.o: ../ETMI2C.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMI2C.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMI2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../ETMI2C.c  -o ${OBJECTDIR}/_ext/1472/ETMI2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/ETMI2C.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/ETMI2C.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/ETMSPI.o: ../ETMSPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMSPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMSPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../ETMSPI.c  -o ${OBJECTDIR}/_ext/1472/ETMSPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/ETMSPI.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/ETMSPI.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/MCP23017.o: ../MCP23017.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/MCP23017.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/MCP23017.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../MCP23017.c  -o ${OBJECTDIR}/_ext/1472/MCP23017.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/MCP23017.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/MCP23017.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/IOPorts.o: ../IOPorts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/IOPorts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/IOPorts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../IOPorts.c  -o ${OBJECTDIR}/_ext/1472/IOPorts.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/IOPorts.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/IOPorts.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/A34760.o: ../A34760.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/A34760.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/A34760.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../A34760.c  -o ${OBJECTDIR}/_ext/1472/A34760.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/A34760.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/A34760.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/faults.o: ../faults.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/faults.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/faults.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../faults.c  -o ${OBJECTDIR}/_ext/1472/faults.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/faults.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/faults.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/Serial.o: ../Serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Serial.c  -o ${OBJECTDIR}/_ext/1472/Serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/Serial.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Serial.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1472/Buffer64.o: ../Buffer64.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Buffer64.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Buffer64.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Buffer64.c  -o ${OBJECTDIR}/_ext/1472/Buffer64.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/Buffer64.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Buffer64.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/ETMdsp.o: ../ETMdsp.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMdsp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMdsp.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../ETMdsp.s  -o ${OBJECTDIR}/_ext/1472/ETMdsp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -I".." -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1472/ETMdsp.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/ETMdsp.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/1472/ETMdsp.o: ../ETMdsp.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMdsp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ETMdsp.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../ETMdsp.s  -o ${OBJECTDIR}/_ext/1472/ETMdsp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -I".." -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1472/ETMdsp.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/ETMdsp.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/A34760.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  C:/Program\ Files\ (x86)/Microchip/xc16/v1.21/lib/libdsp-elf.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/A34760.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    "C:\Program Files (x86)\Microchip\xc16\v1.21\lib\libdsp-elf.a"  -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -Wl,,--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--library-path=".",--no-force-link,--smart-io,-Map="${DISTDIR}/A34760.X.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/A34760.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  C:/Program\ Files\ (x86)/Microchip/xc16/v1.21/lib/libdsp-elf.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/A34760.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    "C:\Program Files (x86)\Microchip\xc16\v1.21\lib\libdsp-elf.a"  -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--library-path=".",--no-force-link,--smart-io,-Map="${DISTDIR}/A34760.X.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
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
