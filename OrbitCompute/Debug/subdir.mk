################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../computepara.cpp \
../dbaccess.cpp \
../func.cpp \
../gSatTEME.cpp \
../gSatWrapper.cpp \
../gTime.cpp \
../gTimeSpan.cpp \
../main.cpp \
../mathUtils.cpp \
../sgp4ext.cpp \
../sgp4io.cpp \
../sgp4unit.cpp 

OBJS += \
./computepara.o \
./dbaccess.o \
./func.o \
./gSatTEME.o \
./gSatWrapper.o \
./gTime.o \
./gTimeSpan.o \
./main.o \
./mathUtils.o \
./sgp4ext.o \
./sgp4io.o \
./sgp4unit.o 

CPP_DEPS += \
./computepara.d \
./dbaccess.d \
./func.d \
./gSatTEME.d \
./gSatWrapper.d \
./gTime.d \
./gTimeSpan.d \
./main.d \
./mathUtils.d \
./sgp4ext.d \
./sgp4io.d \
./sgp4unit.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/Cellar/mysql/5.7.18_1/include/mysql -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


