#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "math.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"

#include "bsp.h"
#include "nordic_common.h"
#include "nrf_drv_clock.h"
#include "sdk_errors.h"

#include "app_error.h"
#include "app_util.h"
#include "app_util_platform.h"

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "nrf_drv_gpiote.h"
#include "nrf_drv_spi.h"
#include "nrf_drv_twi.h"
#include "nrf_pwr_mgmt.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


#define BIT(nr)			(1UL << (nr))
#define GENMASK(h, l)           (((~0UL) - (1UL << (l)) + 1) & (~0UL >> (32 - 1 - (h))))

#define TWI_BUFFER_SIZE 255
#define SPI_BUF_SIZE 255

extern const nrf_drv_twi_t twi0;
extern volatile bool twi_xfer_done;


extern uint8_t p_tx_buf[SPI_BUF_SIZE];
extern uint8_t p_rx_buf[SPI_BUF_SIZE];
extern const nrf_drv_spi_t spi2;
extern volatile bool spi_xfer_done;

#endif //MAIN_H