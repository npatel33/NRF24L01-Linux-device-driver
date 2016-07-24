#ifndef _RADIO_H
#define _RADIO_Hi

#include "gpio.h"
#include <linux/spi/spi.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <mach/platform.h>
#include <linux/slab.h>

#define R 0
#define W 1

#define DATA_PIPE_0 (1<<0)
#define DATA_PIPE_1 (1<<1)
#define DATA_PIPE_2 (1<<2)
#define DATA_PIPE_3 (1<<3)
#define DATA_PIPE_4 (1<<4)
#define DATA_PIPE_5 (1<<5)
#define DATA_PIPE_ALL 0xFF

#define SPEED_1Mbps 0
#define SPEED_2Mbps 1
#define SPEED_250Kbps 4

#define POWER_18DB 0
#define POWER_12DB 1
#define POWER_6DB 2
#define POWER_0DB 3


void radio_init(void);
uint8_t *nrf_xfer(uint8_t reg, size_t count, uint8_t *val, bool op);
void nrf_set_AA(uint8_t pipe);
void nrf_enable_pipes(uint8_t pipe);
int nrf_set_retrans_delay(uint8_t delay);
int nrf_set_addr_width(uint8_t width);
int nrf_set_retrans_count(uint8_t count);
int nrf_set_channel(uint8_t channel);
int nrf_set_speed(uint8_t speed);
int nrf_set_power(uint8_t power);
int nrf_set_rx_addr(uint8_t *val, uint8_t pipe);
void nrf_set_tx_addr(uint8_t *val);
int nrf_set_rx_width(uint8_t width, uint8_t pipe);
int is_tx_full(void);
int is_tx_empty(void);
int is_rx_full(void);
int is_rx_empty(void);
void nrf_enable_dynpd(uint8_t pipe);
void nrf_power_up(void);
void nrf_power_down(void);
void nrf_set_as_recv(void);
void nrf_set_as_trans(void);

#endif
