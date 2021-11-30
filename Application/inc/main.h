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

#include "ads1115.h"


#define BIT(nr)			(1UL << (nr))
#define GENMASK(h, l)           (((~0UL) - (1UL << (l)) + 1) & (~0UL >> (32 - 1 - (h))))

#define TWI_BUFFER_SIZE 255
#define SPI_BUF_SIZE 255

#define TWI0_SCL_PIN ARDUINO_SCL_PIN
#define TWI0_SDA_PIN ARDUINO_SDA_PIN
#define ADS1115_IRQ_PIN NRF_GPIO_PIN_MAP(0,2)

//comment below line if using interrupt of READY pin. 
//#define ADS1115_USE_DELAY


extern const nrf_drv_twi_t twi0;
extern volatile bool twi_xfer_done;


extern uint8_t p_tx_buf[SPI_BUF_SIZE];
extern uint8_t p_rx_buf[SPI_BUF_SIZE];
extern const nrf_drv_spi_t spi2;
extern volatile bool spi_xfer_done;



#endif //MAIN_H