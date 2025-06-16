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
/************************* �Ĵ�����ַ���� *****************************/
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
#define XL2400P_RX_ADDR_P2                      (0X2)        //�洢��RX_ADDR_P2TOP5
#define XL2400P_RX_ADDR_P3                      (0X3)        //�洢��RX_ADDR_P2TOP5
#define XL2400P_RX_ADDR_P4                      (0X4)        //�洢��RX_ADDR_P2TOP5
#define XL2400P_RX_ADDR_P5                      (0X5)        //�洢��RX_ADDR_P2TOP5
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
/*************************** ����ָ��� *****************************/
/*********************************************************************/

#define XL2400P_R_REGISTER                      0x00 //���Ĵ���ָ��                                     
#define XL2400P_W_REGISTER                      0x20 //д�Ĵ���ָ��
#define XL2400P_R_RX_PLOAD                      0x61 //����������ָ��
#define XL2400P_W_TX_PLOAD                      0xA0 //д��������ָ��
#define XL2400P_FLUSH_TX                        0xE1 //���TX-FIFOָ��   
#define XL2400P_FLUSH_RX                        0xE2 //���RX-FIFOָ�� 
#define XL2400P_R_RX_PL_WID                     0x60 //��RX-FIFO���ݳ���ָ��
#define XL2400P_W_ACK_PLOAD                     0xA8
#define XL2400P_W_TX_PLOAD_NOACK                0xB0
#define XL2400P_CMD_NOP                         0xFF //�ղ���

/*
 * ����ͨ��Ƶ��
 */
void xl2400p_set_channel(uint16_t channel);

/*
 * ��ȡCE
 */
bool xl2400p_get_soft_ce(void);

/*
 * ����CE
 */
void xl2400p_set_soft_ce(bool ce_enable);

/*
 * ��ȡ�Ƿ���PRXģʽ
 */
bool xl2400p_get_is_prx(void);

/*
 * �����Ƿ���PRXģʽ
 */
void xl2400p_set_is_prx(bool prx_mode);


/*
 * ��ȡRF״̬
 */
uint8_t xl2400p_get_rf_status(void);

/*
 * ��շ���
 */
void xl2400p_flush_tx(void);

/*
 * ��ս���
 */
void xl2400p_flush_rx(void);

/*
 * ���÷��͵�ַ
 */
void xl2400p_set_tx_addr(const void *addr,size_t addr_len);

/*
* ��ȡ���͵�ַ
 */
void xl2400p_get_tx_addr(void *addr,size_t addr_len);

/*
 * ���ý��յ�ַ(ͨ��0)
 */
void xl2400p_set_rx_addr0(const void *addr,size_t addr_len);

/*
 * ��ȡ���յ�ַ(ͨ��0)
 */
void xl2400p_get_rx_addr0(void *addr,size_t addr_len);

/*
 * ���ý��յ�ַ(ͨ��1)
 */
void xl2400p_set_rx_addr1(const void *addr,size_t addr_len);

/*
 * ��ȡ���յ�ַ(ͨ��1)
 */
void xl2400p_get_rx_addr1(void *addr,size_t addr_len);


/*
 * ���ý��յ�ַ(ͨ��2��5)
 */
void xl2400p_set_rx_addr2top5(const void *addr,size_t addr_len);

/*
 * ��ȡ���յ�ַ(ͨ��2��5)
 */
void xl2400p_get_rx_addr2top5(void *addr,size_t addr_len);


/*
 * ��ȡ���յĸ������ݳ���
 */
uint8_t xl2400p_get_rx_payload_length(uint8_t px);

/*
 * ��ȡ�����յ�ͨ��(-1��ʾ����)
 */
int8_t xl2400p_get_rx_pipe_num(void);

/*
 * дTX FIFO
 */
void xl2400p_write_tx_fifo(const void *data,size_t data_len);


/*
 * дTX FIFO(��ACK),ע��:��Ҫʹ����Ӧ���ܺ���Ч
 */
void xl2400p_write_tx_fifo_noack(const void *data,size_t data_len);

/*
 * ��ȡRX FIFO
 */
void xl2400p_write_rx_fifo(void *data,size_t data_len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
