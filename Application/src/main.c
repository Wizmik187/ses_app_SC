
#include "main.h"

// global variables and defenitions of TWI INSTANCE 0 (for communicating with BQ25150 and DA7280)

const nrf_drv_twi_t twi0 = NRF_DRV_TWI_INSTANCE(0);
volatile bool twi_xfer_done = false;



// global variables and defenitions of SPI INSTANCE 2 (for communicating with NDP)

uint8_t p_tx_buf[SPI_BUF_SIZE];
uint8_t p_rx_buf[SPI_BUF_SIZE];
const nrf_drv_spi_t spi2 = NRF_DRV_SPI_INSTANCE(2);  /**< SPI instance. */
volatile bool spi_xfer_done = false;;

TaskHandle_t debug_mode_Handle = NULL;

#if NRF_LOG_ENABLED
static TaskHandle_t m_logger_thread;                                /**< Definition of Logger thread. */
/**@brief Thread for handling the logger.
 *
 * @details This thread is responsible for processing log entries if logs are deferred.
 *          Thread flushes all log entries and suspends. It is resumed by idle task hook.
 *
 * @param[in]   arg   Pointer used for passing some arbitrary information (context) from the
 *                    osThreadCreate() call to the thread.
 */
static void logger_thread(void * arg)
{
    UNUSED_PARAMETER(arg);

    while (1)
    {
        NRF_LOG_FLUSH();

        vTaskSuspend(NULL); // Suspend myself
    }
}
#endif //NRF_LOG_ENABLED

/**
 * @brief SPI user event handler.
 * @param event
 */
void spi_event_handler(nrf_drv_spi_evt_t const * p_event, void *p_context)
{
    spi_xfer_done = true;
}


void twi_event_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    switch (p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
        case NRF_DRV_TWI_EVT_ADDRESS_NACK:
        case NRF_DRV_TWI_EVT_DATA_NACK:
            twi_xfer_done = true;
            break;
        default:
            break;
    }
}

void da7280_irq_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    BaseType_t checkIfYieldRequired;
    checkIfYieldRequired = xTaskResumeFromISR(haptic_manager_Handle);
    portYIELD_FROM_ISR(checkIfYieldRequired);
}

void bq25150_irq_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    BaseType_t checkIfYieldRequired;
    checkIfYieldRequired = xTaskResumeFromISR(pwr_btn_manager_Handle);
    portYIELD_FROM_ISR(checkIfYieldRequired);
}

void ndp_irq_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    BaseType_t checkIfYieldRequired;
    checkIfYieldRequired = xTaskResumeFromISR(hear_something_Handle);
    portYIELD_FROM_ISR(checkIfYieldRequired);
}

static void hear_something(void * pvParameter);
static void debug_mode(void * pvParameter);
static void pwr_btn_manager(void * pvParameter);
static void haptic_manager(void * pvParameter);

static void gpio_init(void);

/**@brief Function for initializing the nrf log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

/**@brief A function which is hooked to idle task.
 * @note Idle hook must be enabled in FreeRTOS configuration (configUSE_IDLE_HOOK).
 */
void vApplicationIdleHook( void )
{
#if NRF_LOG_ENABLED
     vTaskResume(m_logger_thread);
#endif
}

int main(void)
{
    ret_code_t err_code;
    /* Initialize clock driver for better time accuracy in FREERTOS */
    err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
    log_init();
    gpio_init();

#if NRF_LOG_ENABLED
    // Start execution.
    if (pdPASS != xTaskCreate(logger_thread, "LOGGER", 256, NULL, 1, &m_logger_thread))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
#endif

#ifdef DEBUG
    if (pdPASS != xTaskCreate(debug_mode, "dm", configMINIMAL_STACK_SIZE + 512, NULL, 2, &debug_mode_Handle))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
#endif

    /* Start FreeRTOS scheduler. */
    vTaskStartScheduler();

    while (true)
    {
        /* FreeRTOS should not be here... FreeRTOS goes back to the start of stack
         * in vTaskStartScheduler function. */
        APP_ERROR_HANDLER(NRF_ERROR_FORBIDDEN);
    }
}




static void debug_mode(void * pvParameter)
{

   
    while (1)
    {
        NRF_LOG_INFO("Hello World!");
        nrf_gpio_pin_toggle(LED_1);
        vTaskDelay(1000);
    }
}


static void gpio_init(void)
{
    ret_code_t err_code;

    //nrf_drv_spi_config_t spi2_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    //spi2_config.ss_pin   = SPIM2_SS_PIN;
    //spi2_config.miso_pin = SPIM2_MISO_PIN;
    //spi2_config.mosi_pin = SPIM2_MOSI_PIN;
    //spi2_config.sck_pin  = SPIM2_SCK_PIN;
    //spi2_config.frequency = NRF_DRV_SPI_FREQ_4M;

    //APP_ERROR_CHECK(nrf_drv_spi_init(&spi2, &spi2_config, spi_event_handler, NULL));


    //const nrf_drv_twi_config_t twi0_config = {
    //   .scl                = TWI0_SCL_PIN,
    //   .sda                = TWI0_SDA_PIN,
    //   .frequency          = NRF_DRV_TWI_FREQ_400K,
    //   .interrupt_priority = APP_IRQ_PRIORITY_LOWEST,
    //   .clear_bus_init     = false
    //};

    //err_code = nrf_drv_twi_init(&twi0, &twi0_config, twi_event_handler, NULL);
    //APP_ERROR_CHECK(err_code);
    //nrf_drv_twi_enable(&twi0);
    
    nrf_gpio_cfg_output(LED_1);
    nrf_gpio_cfg_output(LED_2);
    nrf_gpio_cfg_output(LED_3);
    nrf_gpio_cfg_output(LED_4);
    nrf_gpio_pin_clear(LED_1);
    nrf_gpio_pin_clear(LED_2);
    nrf_gpio_pin_clear(LED_3);
    nrf_gpio_pin_clear(LED_4);

    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);
    
}





/**
 *@}
 **/
