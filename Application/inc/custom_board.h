#ifndef CUSTOM_BOARD_H
#define CUSTOM_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"

#define LEDS_NUMBER    3

#define LED_1          25
#define LED_2          22
#define LED_3          23

#define LED_RED        LED_1
#define LED_GREEN      LED_2
#define LED_BLUE       LED_3

#define LEDS_ACTIVE_STATE 0

#define LEDS_INV_MASK  LEDS_MASK

#define LEDS_LIST {LED_1, LED_2, LED_3}

#define BSP_LED_0      LED_1
#define BSP_LED_1      LED_2
#define BSP_LED_2      LED_3

#define BUTTONS_NUMBER 0

#define BUTTONS_ACTIVE_STATE 0

#define TWI0_SCL_PIN     2
#define TWI0_SDA_PIN     3

#define SPIM1_SCK_PIN    15   // Flash, SPI clock GPIO pin number.
#define SPIM1_MOSI_PIN   14   // Flash, SPI Master Out Slave In GPIO pin number.
#define SPIM1_MISO_PIN   16   // Flash, SPI Master In Slave Out GPIO pin number.
#define SPIM1_SS_PIN     18   // Flash, SPI Slave Select GPIO pin number.

#define SPIM2_SCK_PIN    19  // NDP, SPI clock GPIO pin number.
#define SPIM2_MOSI_PIN   11  // NDP, SPI Master Out Slave In GPIO pin number.
#define SPIM2_MISO_PIN   20  // NDP, SPI Master In Slave Out GPIO pin number.
#define SPIM2_SS_PIN     28  // NDP, SPI Slave Select GPIO pin number.

#define NDP_IRQ_PIN      29   // NDP Interrupt pin number.
#define DA7280_IRQ_PIN   24
#define BQ25150_IRQ_PIN  30

#define BQ25150_PG_PIN   27    // BQ25150's nPG pin
#define BQ25150_LP_PIN   31    // BQ25150's nLow_Power pin

#define PDM_CLK_EN       26    // PDM clock enable pin (0: NDP, 1: nRF52)

#define NRF_CLOCK_LFCLKSRC {.source = NRF_CLOCK_LF_SRC_XTAL, \
                            .rc_ctiv = 0, \
                            .rc_temp_ctiv = 0, \
                            .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}

#ifdef __cplusplus
}
#endif

#endif // CUSTOM_BOARD_H

