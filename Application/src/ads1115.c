#include "main.h"

struct ads1115_data ads1115_state;

static ret_code_t ads1115_write(uint8_t const reg, uint16_t val);
static ret_code_t ads1115_read(uint8_t const reg, uint16_t *val);
static ret_code_t ads1115_update_bits(uint8_t reg, uint16_t mask, uint16_t val);

ret_code_t ads1115_get_adc_result(struct ads1115_data *data, enum ads1115_channels chan, enum ads1115_pga pga, enum ads1115_dr dr, uint16_t *val)
{
    ret_code_t err_code = 0;
    uint16_t old = 0, cfg = 0, mask = 0, val_tmp = 0;

    if (chan < ADS1115_AIN0 && chan > ADS1115_AIN3)
        return NRF_ERROR_INVALID_DATA; // only single-ended channels are supported

    err_code = ads1115_read(ADS1115_CFG_REG, &old);
    APP_ERROR_CHECK(err_code);

    data->se_channel_data[chan].pga = pga;
    data->se_channel_data[chan].data_rate = dr;

    mask = ADS1115_CFG_MUX_MASK | ADS1115_CFG_PGA_MASK |
		ADS1115_CFG_DR_MASK;
    cfg = (chan + 4) << ADS1115_CFG_MUX_SHIFT |
            pga << ADS1115_CFG_PGA_SHIFT | 
            dr << ADS1115_CFG_DR_SHIFT;

    cfg = (old & ~mask) | (cfg & mask);

    err_code = ads1115_write(ADS1115_CFG_REG, cfg);
    APP_ERROR_CHECK(err_code);

#ifdef ADS1115_USE_DELAY
    switch (dr)
    {
       case ADS1115_8_SPS:
           vTaskDelay(pdMS_TO_TICKS(126));
           break;
       case ADS1115_16_SPS:
           vTaskDelay(pdMS_TO_TICKS(63));
           break;
       case ADS1115_32_SPS:
           vTaskDelay(pdMS_TO_TICKS(32));
           break;
       case ADS1115_64_SPS:
           vTaskDelay(pdMS_TO_TICKS(16));
           break;
       case ADS1115_128_SPS:
           vTaskDelay(pdMS_TO_TICKS(8));
           break;
       case ADS1115_250_SPS:
           vTaskDelay(pdMS_TO_TICKS(4));
           break;
       case ADS1115_475_SPS:
           vTaskDelay(pdMS_TO_TICKS(3));
           break;
       case ADS1115_860_SPS:
           vTaskDelay(pdMS_TO_TICKS(2));
           break;
    }
#endif

#ifndef ADS1115_USE_DELAY
    vTaskSuspend(NULL);
#endif

    err_code = ads1115_read(ADS1115_CONV_REG, &val_tmp);
    APP_ERROR_CHECK(err_code);

    data->se_channel_data[chan].val = val_tmp;
    *val = val_tmp;

    return err_code;
}
void ads1115_init()
{
    ret_code_t err_code = 0;
    uint16_t cfg = 0;
    
    cfg =   0 << ADS1115_CFG_OS_SHIFT |
            ADS1115_DEFAULT_CHAN << ADS1115_CFG_MUX_SHIFT |
            ADS1115_DEFAULT_PGA << ADS1115_CFG_PGA_SHIFT | 
            ADS1115_SINGLESHOT << ADS1115_CFG_MOD_SHIFT | // Single-shot mode
            ADS1115_DEFAULT_DATA_RATE << ADS1115_CFG_DR_SHIFT |
            ADS1115_CFG_COMP_MODE_TRAD << ADS1115_CFG_COMP_MODE_SHIFT |
            ADS1115_CFG_COMP_POL_LOW << ADS1115_CFG_COMP_POL_SHIFT |
            ADS1115_CFG_COMP_LAT_DISABLE << ADS1115_CFG_COMP_LAT_SHIFT |
            ADS1115_CFG_COMP_ONE << ADS1115_CFG_COMP_QUE_SHIFT;

    
    err_code = ads1115_write(ADS1115_CFG_REG, cfg);
    APP_ERROR_CHECK(err_code);

    // ALERT/RDY pin is configured as a conversion ready pin (See datasheet 9.3.8)
    err_code = ads1115_write(ADS1115_HI_THRESH_REG, 0xFFFF);
    APP_ERROR_CHECK(err_code);
    err_code = ads1115_write(ADS1115_LO_THRESH_REG, 0x0000);
    APP_ERROR_CHECK(err_code);
  
}

static ret_code_t ads1115_write(uint8_t const reg, uint16_t val)
{
    ret_code_t err_code = 0;
    uint8_t reg_tmp[3] = {0, };

    reg_tmp[0] = reg;
    reg_tmp[1] = (val >> 8) & 0xFF;
    reg_tmp[2] = (val >> 0) & 0xFF;    
    
    twi_xfer_done = false;
    
    err_code = nrf_drv_twi_tx(&twi0, ADS1115_ADDR, reg_tmp, 3, false);
    APP_ERROR_CHECK(err_code);

    while (twi_xfer_done == false)
    {
       __WFE();
    }

    return err_code;

}

static ret_code_t ads1115_read(uint8_t const reg, uint16_t *val)
{
    ret_code_t err_code = 0;
    uint8_t reg_tmp[2] = {0, };

    twi_xfer_done = false;
    err_code = nrf_drv_twi_tx(&twi0, ADS1115_ADDR, &reg, 1, false);
    APP_ERROR_CHECK(err_code);

    while (twi_xfer_done == false)
    {
       __WFE();
    }

    twi_xfer_done = false;
    err_code = nrf_drv_twi_rx(&twi0, ADS1115_ADDR, reg_tmp, 2);
    APP_ERROR_CHECK(err_code);

    while (twi_xfer_done == false)
    {
       __WFE();
    }

    *val = (reg_tmp[0] << 8) | (reg_tmp[1] & 0xff);
    return err_code;

}


static ret_code_t ads1115_update_bits(uint8_t reg, uint16_t mask, uint16_t val) 
{ 
    ret_code_t err_code = 0;
    uint16_t tmp, orig;
    uint8_t reg_tmp[2];
    
    err_code = ads1115_read(reg, &orig);
    
    APP_ERROR_CHECK(err_code);

    tmp = orig& ~mask; 
    tmp |= val & mask; 
 
    if (tmp != orig)
    {
        err_code = ads1115_write(reg, tmp);
        APP_ERROR_CHECK(err_code);
    }
 
    return err_code; 
}

