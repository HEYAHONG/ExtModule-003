cmake_minimum_required(VERSION 3.20)
#arm-none-eabi-gcc 工具链设置
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ADDR2LINE arm-none-eabi-addr2line)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_RANLIB arm-none-eabi-ranlib)
set(CMAKE_CXX_COMPILER_AR arm-none-eabi-ar)
set(CMAKE_CXX_COMPILER_RANLIB arm-none-eabi-ranlib)
set(CMAKE_C_COMPILER_AR arm-none-eabi-ar)
set(CMAKE_C_COMPILER_RANLIB arm-none-eabi-ranlib)
set(CMAKE_Fortran_COMPILER arm-none-eabi-gfortan)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(CMAKE_RC_COMPILER arm-none-eabi-windres)
set(CMAKE_READELF arm-none-eabi-readelf)
set(CMAKE_STRIP arm-none-eabi-strip)
set(CMAKE_LINKER arm-none-eabi-ld)
set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)
set(CMAKE_EXECUTABLE_SUFFIX_C .elf)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
#设置工程名称
set(PROJECT_NAME base)
project(${PROJECT_NAME} C CXX ASM)
#设置编译选项(采用newlib的nano.specs)
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} \
		-Wall \
		-Os \
		-DGCC_COMPILE=1 \
		-mthumb \
		-ffunction-sections \
		-fdata-sections \
		-mcpu=cortex-m0plus \
		-std=c11 \
		-march=armv6-m \
		-specs=nano.specs \
		-Wno-restrict\
		-flto\
		 ")
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} \
                -Wall \
		-fno-rtti \
		-Os \
                -DGCC_COMPILE=1 \
		-mthumb \
                -ffunction-sections \
                -fdata-sections \
                -mcpu=cortex-m0plus \
                -std=c++11 \
                -Wno-psabi \
                -march=armv6-m \
		-specs nano.specs \
		-flto \
                 " )
set(CMAKE_EXE_LINKER_FLAGS " ${CMAKE_EXE_LINKER_FLAGS}\
		-mthumb \
		-Wl,--gc-sections \
		-mcpu=cortex-m0plus \
		-T${CMAKE_CURRENT_SOURCE_DIR}/py32f030x8.ld \
		-Wl,--warn-common \
		-Wl,-Map,${PROJECT_NAME}.map \
		-flto \
		")

set(BUILD_SHARED_LIBS OFF)
#设置宏定义
add_definitions(
-DUSE_HAL_DRIVER
-DPY32F030x8
-DHBOX_CONFIG_HEADER=hbox_port.h
-DFORBID_VECT_TAB_MIGRATION=1
)
#设置头文件目录
include_directories(
../../3rdparty/PY32F0xx_Firmware_V1.4.7/Drivers/CMSIS/Include/
../../3rdparty/PY32F0xx_Firmware_V1.4.7/Drivers/CMSIS/Device/PY32F0xx/Include/
../../3rdparty/PY32F0xx_Firmware_V1.4.7/Drivers/PY32F0xx_HAL_Driver/Inc/
../
./
)
#添加工程文件
file(GLOB C_CXX_FILES
../../3rdparty/PY32F0xx_Firmware_V1.4.7/Drivers/CMSIS/Include/*.h
../../3rdparty/PY32F0xx_Firmware_V1.4.7/Drivers/CMSIS/Device/PY32F0xx/Include/*.h
../../3rdparty/PY32F0xx_Firmware_V1.4.7/Drivers/PY32F0xx_HAL_Driver/Src/*.c
../../3rdparty/PY32F0xx_Firmware_V1.4.7/Drivers/PY32F0xx_HAL_Driver/Inc/*.h
../*.s
../*.h
../*.c
../*.cpp
*.s
*.h
*.c
*.cpp
)
#添加可执行文件
add_executable(${PROJECT_NAME} ${C_CXX_FILES})
add_custom_command(TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND  arm-none-eabi-size ${PROJECT_NAME}.elf
	COMMAND  arm-none-eabi-objcopy --verbose -O binary ${PROJECT_NAME}.elf  ${PROJECT_NAME}.bin
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_subdirectory(../../3rdparty/HCppBox/master/hbox HCppBox_hbox)
#启用hbox
hbox_enable(${PROJECT_NAME})


