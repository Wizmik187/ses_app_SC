#ifndef ADS1115_H
#define ADS1115_H


/*
 * IIO driver for ADS1115 ADC 7-bit I2C slave address:
 *	* 0x48 - ADDR connected to Ground
 *	* 0x49 - ADDR connected to Vdd
 *	* 0x4A - ADDR connected to SDA
 *	* 0x4B - ADDR connected to SCL
 */
#define ADS1115_ADDR	(0x48)

#define ADS1115_SE_CHANNELS 4

#define ADS1115_CONV_REG	0x00
#define ADS1115_CFG_REG		0x01
#define ADS1115_LO_THRESH_REG	0x02
#define ADS1115_HI_THRESH_REG	0x03

#define ADS1115_CFG_COMP_QUE_SHIFT	0
#define ADS1115_CFG_COMP_LAT_SHIFT	2
#define ADS1115_CFG_COMP_POL_SHIFT	3
#define ADS1115_CFG_COMP_MODE_SHIFT	4
#define ADS1115_CFG_DR_SHIFT	5
#define ADS1115_CFG_MOD_SHIFT	8
#define ADS1115_CFG_PGA_SHIFT	9
#define ADS1115_CFG_MUX_SHIFT	12
#define ADS1115_CFG_OS_SHIFT	15

#define ADS1115_CFG_COMP_QUE_MASK	GENMASK(1, 0)
#define ADS1115_CFG_COMP_LAT_MASK	BIT(2)
#define ADS1115_CFG_COMP_POL_MASK	BIT(3)
#define ADS1115_CFG_COMP_MODE_MASK	BIT(4)
#define ADS1115_CFG_DR_MASK	GENMASK(7, 5)
#define ADS1115_CFG_MOD_MASK	BIT(8)
#define ADS1115_CFG_PGA_MASK	GENMASK(11, 9)
#define ADS1115_CFG_MUX_MASK	GENMASK(14, 12)


#define ADS1115_DEFAULT_PGA		ADS1115_FSR_2048
#define ADS1115_DEFAULT_DATA_RATE	ADS1115_128_SPS
#define ADS1115_DEFAULT_CHAN		(ADS1115_AIN0 + 4)


/* Input multiplexer configuration 
   only Single-ended channels are used*/
enum ads1115_channels {
	ADS1115_AIN0 = 0,
	ADS1115_AIN1,
	ADS1115_AIN2,
	ADS1115_AIN3,
};

/* Programmable gain amplifier configuration */
enum ads1115_pga {
	ADS1115_FSR_6144 = 0,
	ADS1115_FSR_4096,
	ADS1115_FSR_2048,
	ADS1115_FSR_1024,
	ADS1115_FSR_0512,
	ADS1115_FSR_0256,
	ADS1115_FSR_0256_1,
	ADS1115_FSR_0256_2,
};

/* device operating modes */
#define ADS1115_CONTINUOUS	0
#define ADS1115_SINGLESHOT	1

/* Data rate */
enum ads1115_dr {
	ADS1115_8_SPS = 0,
	ADS1115_16_SPS,
	ADS1115_32_SPS,
	ADS1115_64_SPS,
	ADS1115_128_SPS,
	ADS1115_250_SPS,
	ADS1115_475_SPS,
	ADS1115_860_SPS,
};

/* Comparator mode field */
#define ADS1115_CFG_COMP_MODE_TRAD	0
#define ADS1115_CFG_COMP_MODE_WINDOW	1

/* Comparator polarity field */
#define ADS1115_CFG_COMP_POL_LOW	0
#define ADS1115_CFG_COMP_POL_HIGH	1

/* Latching comparator field */
#define ADS1115_CFG_COMP_LAT_DISABLE	0
#define ADS1115_CFG_COMP_LAT_ENABLE		1

/* Comparator queue and disable field */
#define ADS1115_CFG_COMP_ONE	0
#define ADS1115_CFG_COMP_TWO	1
#define ADS1115_CFG_COMP_FOUR	2
#define ADS1115_CFG_COMP_DISABLE	3


struct ads1115_channel_data {
	enum ads1115_pga pga;
	enum ads1115_dr data_rate;
        uint16_t val;
};

struct ads1115_data {
	struct ads1115_channel_data se_channel_data[ADS1115_SE_CHANNELS];

};

extern struct ads1115_data ads1115_state;


void ads1115_init();
ret_code_t ads1115_get_adc_result(struct ads1115_data *data, enum ads1115_channels chan, enum ads1115_pga pga, enum ads1115_dr dr, uint16_t *val);


#endif //ADS1115_H