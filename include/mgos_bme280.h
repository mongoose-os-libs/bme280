#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
struct mgos_bme280;
struct mgos_bme280_stats {
  double last_read_time;         // value of mg_time() upon last call to _read()
  uint32_t read;                 // calls to _read()
  uint32_t read_success;         // successful _read()
  uint32_t read_success_cached;  // calls to _read() which were cached
  // Note: read_errors := read - read_success - read_success_cached
  double read_success_usecs;     // time spent in successful uncached _read()
};


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

/*
 * Creates a `mgos_bme280_data` struct to be used in js
 */
struct mgos_bme280_data* mgos_bme280_data_create();

/*
 * Frees the struct used by js
 */
void mgos_bme280_data_delete(struct mgos_bme280_data* data);

/*
 * Gets the `temp` member of the mgos_bme280_data struct
 */
double mgos_bme280_data_get_temp(const struct mgos_bme280_data* data);

/*
 * Gets the `press` member of the mgos_bme280_data struct
 */
double mgos_bme280_data_get_press(const struct mgos_bme280_data* data);

/*
 * Gets the `humid` member of the mgos_bme280_data struct
 */
double mgos_bme280_data_get_humid(const struct mgos_bme280_data* data);

/*
 * Returns the running statistics on the sensor interaction, the user provides
 * a pointer to a `struct mgos_bme280_stats` object, which is filled in by this
 * call.
 *
 * Upon success, true is returned. Otherwise, false is returned, in which case
 * the contents of `stats` is undetermined.
 */
bool mgos_bme280_getStats(struct mgos_bme280 *bme, struct mgos_bme280_stats *stats);

#ifdef __cplusplus
}
#endif

