# 说明

XL2409可看作MCU与[XL2400P](https://www.xinlinggo.com/pd.jsp?id=42)通过SIP封装在一起，其固件部分主要使用MCU的开发环境。

本工程主要使用PY32F030X8相关软件开发资源(同官方案例中使用的MCU型号)，具体硬件资源如下:

- 64K Flash
- 8K SRAM

# 工具

主要使用的操作系统:

- Windows 10及更新版本:Keil MDK5开发MCU程序。

## Keil MDK5

MDK5为集成开发环境，可对MCU的程序进行编译、调试等

MDK5的最低版本为5.29。

### 开发环境配置

安装好Keil MDK5并成功激活后，还需要安装设备支持包[Puya.PY32F0xx_DFP.1.2.3.pack](3rdparty/PY32F0xx_Firmware_V1.4.7/Packs/MDK/Puya.PY32F0xx_DFP.1.2.3.pack)

## arm-none-eabi-gcc

arm-none-eabi-gcc主要使用采用newlib-nano作为C运行库的工具，本工程中一般配合CMake使用.

# 目录说明

- [数字]-[名称]：数字为固件编号，名称为固件名称。
- [3rdparty](3rdparty):第三方源代码。

# 固件列表

- [001-base](001-base):基础模板，采用Keil MDK5编译。