#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
struct mgos_bme280;

/*
 * `temp` - temperature in degrees Celsius
 * `press` - pressure in Pa
 * `humid` - humidity in %
 */
struct mgos_bme280_data
{
    double temp;
    double press;
    double humid;
};

extern const double MGOS_BME280_ERROR;

/*
 * Creates a `struct mgos_bme280` for the device with I2C `addr` address
 * Uses global i2c. If different pins than the default ones are used,
 * the user should define them in mos.yml. Eg.
 * ```
 *  - ["i2c.sda_gpio", 12]
 *  - ["i2c.scl_gpio", 14]
 * ```
 * Returns opaque handle (NULL if an error occurred)
 */
struct mgos_bme280* mgos_bme280_i2c_create(uint8_t addr);

/*
 * Creates a `struct mgos_bme280` for the device
 * Uses global spi with `spi.cs0_gpio`
 * If different pins than the default ones are used,
 * the user should define them in mos.yml. Eg.
 * ```
 *  - ["spi.miso_gpio", 19]
 *  - ["spi.mosi_gpio", 23]
 *  - ["spi.sclk_gpio", 18]
 *  - ["spi.cs0_gpio", 5]
 * ```
 * Returns opaque handle (NULL if an error occurred)
 */
struct mgos_bme280* mgos_bme280_spi_create();

/*
 * Deletes the handle and frees resources.
 */
void mgos_bme280_delete(struct mgos_bme280* bme);

/*
 * Reads the temperature, pressure and humidity in the provided `data` structure.
 * If the device is BMP280, the humidity will be 0.
 * Returns 0 if success
 */
int8_t mgos_bme280_read(struct mgos_bme280* bme, struct mgos_bme280_data* data);

/*
 * Reads the temperature.
 * Returns MGOS_BME280_ERROR if error.
 */
double mgos_bme280_read_temperature(struct mgos_bme280* bme);

/*
 * Reads the pressure.
 * Returns MGOS_BME280_ERROR if error.
 */
double mgos_bme280_read_pressure(struct mgos_bme280* bme);

/*
 * Reads the humidity.
 * If the device is BMP280, the humidity will be 0.
 * Returns MGOS_BME280_ERROR if error.
 */
double mgos_bme280_read_humidity(struct mgos_bme280* bme);

/*
 * Returns true if a BME280 device is connected
 */
bool mgos_bme280_is_bme280(struct mgos_bme280* bme);

#ifdef __cplusplus
}
#endif

