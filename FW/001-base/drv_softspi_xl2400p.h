#ifndef __DRV_SOFTSPI_XL2400P_H__
#define __DRV_SOFTSPI_XL2400P_H__
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

void hsoftspi_xl2400p_write_register(uint8_t RF_Reg,uint8_t W_Data);
uint8_t hsoftspi_xl2400p_read_register(uint8_t RF_Reg);
void hsoftspi_xl2400p_write_register_buffer(uint8_t RF_Reg, uint8_t *pBuff, uint8_t Len);
void hsoftspi_xl2400p_read_register_buffer(uint8_t RF_Reg, uint8_t *pBuff, uint8_t Len);


/*********************************************************************/
/************************* 寄存器地址定义 *****************************/
/*********************************************************************/

#define XL2400P_CFG_TOP                         (0X00)
#define XL2400P_EN_AA                           (0X01)
#define XL2400P_EN_RXADDR                       (0X02)
#define XL2400P_SETUP_AW                        (0X03)
#define XL2400P_SETUP_RETR                      (0X04)
#define XL2400P_RF_CH                           (0X05)
#define XL2400P_RF_SETUP                        (0X06)
#define XL2400P_RF_STATUS                       (0X07)
#define XL2400P_OBSERVE_TX                      (0X08)
#define XL2400P_RSSI                            (0X09)
#define XL2400P_RX_ADDR_P0                      (0X0A)
#define XL2400P_RX_ADDR_P1                      (0X0B)
#define XL2400P_RX_ADDR_P2                      (0X2)        //存储在RX_ADDR_P2TOP5
#define XL2400P_RX_ADDR_P3                      (0X3)        //存储在RX_ADDR_P2TOP5
#define XL2400P_RX_ADDR_P4                      (0X4)        //存储在RX_ADDR_P2TOP5
#define XL2400P_RX_ADDR_P5                      (0X5)        //存储在RX_ADDR_P2TOP5
#define XL2400P_RX_ADDR_P2TOP5                  (0X0C)
#define XL2400P_BER_RESULT                      (0X0D)
#define XL2400P_AGC_SETTING                     (0X0E)
#define XL2400P_PGA_SETTING                     (0X0F)
#define XL2400P_TX_ADDR                         (0X10)
#define XL2400P_RX_PW_PX                        (0X11)
#define XL2400P_ANALOG_CFG0                     (0X12)
#define XL2400P_ANALOG_CFG1                     (0X13)
#define XL2400P_ANALOG_CFG2                     (0X14)
#define XL2400P_ANALOG_CFG3                     (0X15)
#define XL2400P_STATUS_FIFO                     (0X17)
#define XL2400P_RSSIREC                         (0X18)
#define XL2400P_TXPROC_CFG                      (0X19)
#define XL2400P_RXPROC_CFG                      (0X1A)
#define XL2400P_DYNPD                           (0X1C)
#define XL2400P_FEATURE                         (0X1D)
#define XL2400P_RAMP_CFG                        (0X1E)

/*********************************************************************/
/*************************** 操作指令定义 *****************************/
/*********************************************************************/

#define XL2400P_R_REGISTER                      0x00 //读寄存器指令                                     
#define XL2400P_W_REGISTER                      0x20 //写寄存器指令
#define XL2400P_R_RX_PLOAD                      0x61 //读接收数据指令
#define XL2400P_W_TX_PLOAD                      0xA0 //写发射数据指令
#define XL2400P_FLUSH_TX                        0xE1 //清空TX-FIFO指令   
#define XL2400P_FLUSH_RX                        0xE2 //清空RX-FIFO指令 
#define XL2400P_R_RX_PL_WID                     0x60 //读RX-FIFO数据长度指令
#define XL2400P_W_ACK_PLOAD                     0xA8
#define XL2400P_W_TX_PLOAD_NOACK                0xB0
#define XL2400P_CMD_NOP                         0xFF //空操作

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
