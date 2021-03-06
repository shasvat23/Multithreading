#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/FileLocking.o \
	${OBJECTDIR}/MutexSupport.o \
	${OBJECTDIR}/ResourceLock.o \
	${OBJECTDIR}/SemaphoreEvents.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++17
CXXFLAGS=-std=c++17

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/multithreading

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/multithreading: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/multithreading ${OBJECTFILES} ${LDLIBSOPTIONS} -pthread

${OBJECTDIR}/FileLocking.o: FileLocking.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Src_Common_Linux/Etc -I/Src_Common_Linux/Inc -I. -I/Src_Common_Linux/Usr -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FileLocking.o FileLocking.cpp

${OBJECTDIR}/MutexSupport.o: MutexSupport.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Src_Common_Linux/Etc -I/Src_Common_Linux/Inc -I. -I/Src_Common_Linux/Usr -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MutexSupport.o MutexSupport.cpp

${OBJECTDIR}/ResourceLock.o: ResourceLock.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Src_Common_Linux/Etc -I/Src_Common_Linux/Inc -I. -I/Src_Common_Linux/Usr -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ResourceLock.o ResourceLock.cpp

${OBJECTDIR}/SemaphoreEvents.o: SemaphoreEvents.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Src_Common_Linux/Etc -I/Src_Common_Linux/Inc -I. -I/Src_Common_Linux/Usr -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SemaphoreEvents.o SemaphoreEvents.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Src_Common_Linux/Etc -I/Src_Common_Linux/Inc -I. -I/Src_Common_Linux/Usr -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
