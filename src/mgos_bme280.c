#include <mgos.h>

#include <mgos_i2c.h>
#include <mgos_spi.h>

#include "mgos_bme280.h"

#include "BME280_driver/bme280.h"

const double MGOS_BME280_ERROR = -128.0;

static int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    /*
     * The parameter dev_id can be used as a variable to store the I2C address of the device
     */

    /*
     * Data on the bus should be like
     * |------------+---------------------|
     * | I2C action | Data                |
     * |------------+---------------------|
     * | Start      | -                   |
     * | Write      | (reg_addr)          |
     * | Stop       | -                   |
     * | Start      | -                   |
     * | Read       | (reg_data[0])       |
     * | Read       | (....)              |
     * | Read       | (reg_data[len - 1]) |
     * | Stop       | -                   |
     * |------------+---------------------|
     */
    //int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */
    struct mgos_i2c* i2c = mgos_i2c_get_global();
    if (NULL == i2c) {
        LOG(LL_INFO, ("Could not get i2c global instance"));
        return -1;
    }
    bool ok = mgos_i2c_read_reg_n(i2c, dev_id, reg_addr, len, reg_data);
    return ok ? 0 : -2;
}

static int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    /*
     * The parameter dev_id can be used as a variable to store the I2C address of the device
     */

    /*
     * Data on the bus should be like
     * |------------+---------------------|
     * | I2C action | Data                |
     * |------------+---------------------|
     * | Start      | -                   |
     * | Write      | (reg_addr)          |
     * | Write      | (reg_data[0])       |
     * | Write      | (....)              |
     * | Write      | (reg_data[len - 1]) |
     * | Stop       | -                   |
     * |------------+---------------------|
     */
    //int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    struct mgos_i2c* i2c = mgos_i2c_get_global();
    if (NULL == i2c) {
        LOG(LL_INFO, ("Could not get i2c global instance"));
        return -1;
    }

    bool ok = mgos_i2c_write_reg_n(i2c, dev_id, reg_addr, len, reg_data);
    return ok ? 0 : -2;
}

static int8_t user_spi_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    /*
     * The parameter dev_id can be used as a variable to select which Chip Select pin has
     * to be set low to activate the relevant device on the SPI bus
     */

    /*
     * Data on the bus should be like
     * |----------------+---------------------+-------------|
     * | MOSI           | MISO                | Chip Select |
     * |----------------+---------------------|-------------|
     * | (don't care)   | (don't care)        | HIGH        |
     * | (reg_addr)     | (don't care)        | LOW         |
     * | (don't care)   | (reg_data[0])       | LOW         |
     * | (....)         | (....)              | LOW         |
     * | (don't care)   | (reg_data[len - 1]) | LOW         |
     * | (don't care)   | (don't care)        | HIGH        |
     * |----------------+---------------------|-------------|
     */
    //int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */
    (void) dev_id; /* Not used. Using default spi.cs0_gpio */

    struct mgos_spi* spi = mgos_spi_get_global();
    if (NULL == spi) {
        LOG(LL_INFO, ("Could not get SPI global instance"));
        return -1;
    }

    struct mgos_spi_txn txn;
    memset(&txn, 0, sizeof (txn));
    txn.cs = 0; /* Using default spi.cs0_gpio */
    txn.mode = 0; /* Mode 0 or 3*/
    txn.freq = 1000000;
    txn.hd.tx_data = &reg_addr;
    txn.hd.tx_len = 1;
    txn.hd.rx_data = reg_data;
    txn.hd.rx_len = len;

    if (!mgos_spi_run_txn(spi, false, &txn)) {
        LOG(LL_INFO, ("SPI transaction failed"));
        return -1;
    }

    return BME280_OK;
}

static int8_t user_spi_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    /*
     * The parameter dev_id can be used as a variable to select which Chip Select pin has
     * to be set low to activate the relevant device on the SPI bus
     */

    /*
     * Data on the bus should be like
     * |---------------------+--------------+-------------|
     * | MOSI                | MISO         | Chip Select |
     * |---------------------+--------------|-------------|
     * | (don't care)        | (don't care) | HIGH        |
     * | (reg_addr)          | (don't care) | LOW         |
     * | (reg_data[0])       | (don't care) | LOW         |
     * | (....)              | (....)       | LOW         |
     * | (reg_data[len - 1]) | (don't care) | LOW         |
     * | (don't care)        | (don't care) | HIGH        |
     * |---------------------+--------------|-------------|
     */
    //int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    (void) dev_id; /* Not used. Using default spi.cs0_gpio */

    struct mgos_spi* spi = mgos_spi_get_global();
    if (NULL == spi) {
        LOG(LL_INFO, ("user_spi_write: Could not get SPI global instance"));
        return -1;
    }

    struct mgos_spi_txn txn;
    memset(&txn, 0, sizeof (txn));
    txn.cs = 0; /* Using default spi.cs0_gpio */
    txn.mode = 0; /* Mode 0 or 3*/
    txn.freq = 1000000;

    uint8_t temp_buff[20]; /* Typically not to write more than 10 registers */
    temp_buff[0] = reg_addr;
    temp_buff[1] = reg_data[0];
    if (len >= 2) {
        memcpy(temp_buff + 2, reg_data + 1, len - 1);
    }
    txn.hd.tx_data = temp_buff;
    txn.hd.tx_len = len + 1;

    if (!mgos_spi_run_txn(spi, false, &txn)) {
        LOG(LL_INFO, ("user_spi_write: SPI transaction failed"));
        return -1;
    }

    return BME280_OK;
}

struct mgos_bme280 {
    struct bme280_dev dev;
};

static int8_t commonInit(struct mgos_bme280* bme)
{
    int8_t rslt = bme280_init(&bme->dev);
    if (BME280_OK != rslt) {
        LOG(LL_INFO, ("BMP/BME280 device not found - %hhd", rslt));
        return rslt;
    }

    /* Recommended mode of operation: Indoor navigation */
    bme->dev.settings.osr_h = BME280_OVERSAMPLING_1X;
    bme->dev.settings.osr_p = BME280_OVERSAMPLING_16X;
    bme->dev.settings.osr_t = BME280_OVERSAMPLING_2X;
    bme->dev.settings.filter = BME280_FILTER_COEFF_16;
    bme->dev.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

    uint8_t settings_sel = BME280_OSR_PRESS_SEL;
    settings_sel |= BME280_OSR_TEMP_SEL;
    if (BME280_CHIP_ID == bme->dev.chip_id) {
        settings_sel |= BME280_OSR_HUM_SEL;
    }
    settings_sel |= BME280_STANDBY_SEL;
    settings_sel |= BME280_FILTER_SEL;
    rslt = bme280_set_sensor_settings(settings_sel, &bme->dev);
    if (BME280_OK != rslt) {
        LOG(LL_INFO, ("Could not set sensor settings"));
        return rslt;
    }

    rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, &bme->dev);
    if (BME280_OK != rslt) {
        LOG(LL_INFO, ("Could not set sensor mode"));
        return rslt;
    }
    return BME280_OK;
}

struct mgos_bme280* mgos_bme280_i2c_create(uint8_t addr)
{
    // Is I2C enabled?
    if (!mgos_sys_config_get_i2c_enable()) {
        LOG(LL_INFO, ("I2C is disabled."));
        return NULL;
    }

    struct mgos_bme280* bme = calloc(1, sizeof (struct mgos_bme280));
    if (NULL == bme) {
        LOG(LL_INFO, ("Could not allocate mgos_bme280 structure."));
        return NULL;
    }

    //initialize the structure
    bme->dev.dev_id = addr;
    bme->dev.intf = BME280_I2C_INTF;
    bme->dev.read = user_i2c_read;
    bme->dev.write = user_i2c_write;
    bme->dev.delay_ms = mgos_msleep;

    int8_t rslt = commonInit(bme);
    if (BME280_OK != rslt) {
        free(bme);
        return NULL;
    }

    return bme;
}

struct mgos_bme280* mgos_bme280_spi_create()
{
    // Is I2C enabled?
    if (!mgos_sys_config_get_spi_enable()) {
        LOG(LL_INFO, ("SPI is disabled."));
        return NULL;
    }

    struct mgos_bme280* bme = calloc(1, sizeof (struct mgos_bme280));
    if (NULL == bme) {
        LOG(LL_INFO, ("Could not allocate mgos_bme280 structure."));
        return NULL;
    }

    //initialize the structure
    bme->dev.dev_id = 0;
    bme->dev.intf = BME280_SPI_INTF;
    bme->dev.read = user_spi_read;
    bme->dev.write = user_spi_write;
    bme->dev.delay_ms = mgos_msleep;

    int8_t rslt = commonInit(bme);
    if (BME280_OK != rslt) {
        free(bme);
        return NULL;
    }

    return bme;
}

int8_t mgos_bme280_read(struct mgos_bme280* bme, struct mgos_bme280_data* data)
{
    if (NULL == bme) {
        return -1;
    }
    /* Don't try to read humidity if BMP280 */
    uint8_t sensor_comp = (BME280_CHIP_ID == bme->dev.chip_id) ? BME280_ALL : (BME280_PRESS | BME280_TEMP);
    struct bme280_data comp_data;
    int8_t rslt = bme280_get_sensor_data(sensor_comp, &comp_data, &bme->dev);
    if (BME280_OK == rslt) {
#ifdef BME280_FLOAT_ENABLE
        data->temp = comp_data.temperature;
        data->press = comp_data.pressure;
        data->humid = comp_data.humidity;
#else
        data->temp = comp_data.temperature / 100.0;
        data->press = comp_data.pressure / 100.0;
        data->humid = comp_data.humidity / 1024.0;
#endif
    }
    return rslt;
}

double mgos_bme280_read_temperature(struct mgos_bme280* bme)
{
    struct bme280_data comp_data;
    int8_t rslt = bme280_get_sensor_data(BME280_TEMP, &comp_data, &bme->dev);
    double result;
    if (BME280_OK == rslt) {
#ifdef BME280_FLOAT_ENABLE
        result = comp_data.temperature;
#else
        result = comp_data.temperature / 100.0;
#endif
    } else {
        result = MGOS_BME280_ERROR;
    }
    return result;
}

double mgos_bme280_read_pressure(struct mgos_bme280* bme)
{
    struct bme280_data comp_data;
    int8_t rslt = bme280_get_sensor_data(BME280_PRESS, &comp_data, &bme->dev);
    double result;
    if (BME280_OK == rslt) {
#ifdef BME280_FLOAT_ENABLE
        result = comp_data.pressure;
#else
        result = comp_data.pressure / 100.0;
#endif
    } else {
        result = MGOS_BME280_ERROR;
    }
    return result;
}

double mgos_bme280_read_humidity(struct mgos_bme280* bme)
{
    if (NULL == bme) {
        return MGOS_BME280_ERROR;
    }
    /* Check if the device is BMP280*/
    if (BME280_CHIP_ID != bme->dev.chip_id) {
        return 0.0;
    }
    struct bme280_data comp_data;
    int8_t rslt = bme280_get_sensor_data(BME280_HUM, &comp_data, &bme->dev);
    double result;
    if (BME280_OK == rslt) {
#ifdef BME280_FLOAT_ENABLE
        result = comp_data.humidity;
#else
        result = comp_data.humidity / 1024.0;
#endif
    } else {
        result = MGOS_BME280_ERROR;
    }
    return result;
}

void mgos_bme280_delete(struct mgos_bme280* bme)
{
    if (NULL != bme) {
        free(bme);
    }
}

bool mgos_bme280_is_bme280(struct mgos_bme280* bme)
{
    if (NULL != bme) {
        return BME280_CHIP_ID == bme->dev.chip_id;
    }
    return false;
}

/*
 */
struct mgos_bme280_data* mgos_bme280_data_create()
{
    return calloc(1, sizeof (struct mgos_bme280_data));
}

/*
 */
void mgos_bme280_data_delete(struct mgos_bme280_data* data)
{
    if (NULL != data) {
        free(data);
    }
}

double mgos_bme280_data_get_temp(const struct mgos_bme280_data* data)
{
    return (NULL != data) ? data->temp : MGOS_BME280_ERROR;
}

/*
 */
double mgos_bme280_data_get_press(const struct mgos_bme280_data* data)
{
    return (NULL != data) ? data->press : MGOS_BME280_ERROR;
}

/*
 */
double mgos_bme280_data_get_humid(const struct mgos_bme280_data* data)
{
    return (NULL != data) ? data->humid : MGOS_BME280_ERROR;
}
